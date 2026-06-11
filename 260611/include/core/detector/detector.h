#pragma once
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <json.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "../model/yolov9_model.h"
#include "../inference/yolo_inference.h"
#include "../output/detection_result.h"
#include "../utils/logger.h"

namespace yolo {

struct DetectorOptions {
    std::string output_dir = "./output";
    std::string model_path = "";
    float conf_threshold = 0.5f;
    float iou_threshold = 0.5f;
    bool draw_boxes = true;
    bool draw_labels = true;
    bool save_results = true;
    bool export_json = false;
    int num_threads = std::thread::hardware_concurrency();
    float fps_target = 30.0f;
    bool show_fps = true;
    std::string camera_id = "0";
    bool loop = false;
    int max_frames = -1;
};

class Detector {
public:
    Detector(const std::string& model_path, const std::string& output_dir,
             DetectorOptions& options = DetectorOptions());
    ~Detector() = default;

    class DetectorResult {
    public:
        std::vector<DetectionResult> detections;
        std::vector<std::string> class_names;
        float fps;
        float avg_inference_time;
        int total_images;
        int total_detections;
        bool success = false;
        std::string error_message;
    };

    DetectorResult detect(const std::string& image_path);
    std::vector<DetectorResult> detect_directory(const std::string& directory_path);
    DetectorResult detect_webcam(int camera_id = 0);
    DetectorResult detect_video(const std::string& video_path);
    DetectorResult detect_batch(const std::vector<std::string>& image_paths);

    void draw_results(cv::Mat& image, std::vector<DetectionResult>& results);
    bool save_results(const cv::Mat& image, const std::string& filename);
    bool export_json(const std::string& path, std::vector<DetectionResult>& results);

    void set_conf_threshold(float threshold) { conf_threshold_ = threshold; }
    float get_conf_threshold() const { return conf_threshold_; }
    float get_iou_threshold() const { return iou_threshold_; }

private:
    void validate_setup();
    std::unique_ptr<YOLOModel> load_model(const std::string& path);
    std::unique_ptr<YOLOInference> initialize_inference();
    std::vector<DetectionResult> infer_image(const std::string& path);
    std::vector<DetectionResult> infer_batch_images(const std::vector<std::string>& paths);
    
    void draw_bounding_box(cv::Mat& image, const DetectionResult& result,
                          float conf, const std::string& class_name);
    bool is_valid_path(const std::string& path);
    std::string format_timestamp(const cv::Mat& image);
    cv::Scalar color_for_class(int class_id);

private:
    std::string model_path_;
    std::string output_dir_;
    YOLOModel* model_;
    YOLOInference* inference_;
    float conf_threshold_ = 0.5f;
    float iou_threshold_ = 0.5f;
    DetectorOptions options_;
    std::string class_names_[80]; // COCO classes
    std::vector<cv::Scalar> class_colors_;
    std::chrono::steady_clock::time_point last_stats_;
    float fps_ = 0.0f;
    int frame_count_ = 0;
    bool initialized_ = false;
};

} // namespace yolo
