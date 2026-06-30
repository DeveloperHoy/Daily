#include "detector.h"
#include <iostream>
#include <opencv2/videoio.hpp>
#include <chrono>

namespace yolo {

Detector::Detector(const std::string& model_path, const std::string& output_dir,
                   DetectorOptions& options)
    : model_path_(model_path), output_dir_(output_dir), options_(options) {
    validate_setup();
    model_ = std::make_unique<YOLOv9Model>(model_path, config_);
    inference_ = std::make_unique<YOLOInference>(std::move(*model_));
    initialized_ = true;
}

DetectorResult Detector::detect(const std::string& image_path) {
    ImageHandler handler;
    if (!handler.open_image(image_path)) {
        return DetectorResult{"Image not found: " + image_path};
    }

    std::vector<DetectionResult> results = inference_->infer(handler.get_next_image());
    
    cv::Mat output_img = handler.get_next_image().clone();
    draw_results(output_img, results);

    DetectorResult result;
    result.detections = results;
    result.success = handler.get_next_image().empty() == false;

    return result;
}

DetectorResult Detector::detect_directory(const std::string& directory_path) {
    return detect_batch(directory_path);
}

DetectorResult Detector::detect_video(const std::string& video_path) {
    // Video processing implementation
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        return DetectorResult{"Video not found"};
    }

    DetectorResult result;
    result.success = true;
    result.total_images = 0;

    while (cap.read()) {
        result.total_images++;
        cv::Mat frame = cap.read();
        cv::Mat output_frame = frame.clone();
        std::vector<DetectionResult> detections = inference_->infer(frame);
        draw_results(output_frame, detections);
        
        if (options_.save_results && result.save_results) {
            cv::imwrite(output_path_ + "_frame_" + std::to_string(result.total_images) + ".jpg", output_frame);
        }
        
        if (!options_.loop) {
            cap.release();
            break;
        }
    }

    return result;
}

bool Detector::draw_results(cv::Mat& image, std::vector<DetectionResult>& results) {
    for (const auto& result : results) {
        float conf = result.confidence;
        std::string label = result.class_name_ + " " + std::to_string(conf);
        cv::Scalar color = cv::Scalar::all(0);

        // Create bounding box
        cv::rectangle(image, result.box_[0], result.box_[1], color, 2);

        // Create label background
        cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2);
        cv::rectangle(image, 
                      result.box_[0], 
                      cv::Point(result.box_[1] + label_size.width, result.box_[1] + label_size.height), 
                      color, -1);

        // Write text
        cv::putText(image, label, result.box_[1], cv::FONT_HERSHEY_SIMPLEX, 0.5, 255);
    }

    return true;
}

bool Detector::export_json(const std::string& path, std::vector<DetectionResult>& results) {
    JsonWriter writer;
    return writer.write_json(path, results);
}

} // namespace yolo
