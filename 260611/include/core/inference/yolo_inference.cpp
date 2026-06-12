#include "yolo_inference.h"
#include <algorithm>
#include <cmath>

namespace yolo {

YOLOInference::YOLOInference(std::unique_ptr<YOLOv9Model> model)
    : model_(std::move(model)) {
    // Initialize inference parameters
}

float YOLOInference::calculate_iou(const DetectionResult& a, const DetectionResult& b) {
    std::pair<int, int> rect_a = {a.box_x_y[0], a.box_x_y[1]};
    std::pair<int, int> rect_b = {b.box_x_y[0], b.box_x_y[1]};

    int x1 = max(rect_a.first, rect_b.first);
    int y1 = max(rect_a.second, rect_b.second);
    int x2 = min(rect_a.first + rect_a.second, rect_b.first + rect_b.second);
    int y2 = min(rect_a.second + rect_a.second, rect_b.second + rect_b.second);

    int w = x2 - x1;
    int h = y2 - y1;

    return (w > 0 && h > 0) ? (float)(w * h) / (rect_a.first * rect_a.second + rect_b.first * rect_b.second) : 0.0f;
}

std::vector<DetectionResult> YOLOInference::non_max_suppression(
    std::vector<DetectionResult>& results, float iou_threshold) {
    std::vector<DetectionResult> filtered;
    std::vector<int> keep_indices;

    for (size_t i = 0; i < results.size(); ++i) {
        bool remove = false;
        for (size_t j = 0; j < results.size(); ++j) {
            if (i == j) continue;
            if (results[j].confidence > results[i].confidence) continue;
            if (calculate_iou(results[i], results[j]) > iou_threshold) {
                remove = true;
                break;
            }
        }
        if (!remove) {
            filtered.push_back(results[i]);
            keep_indices.push_back(i);
        }
    }

    return filtered;
}

std::vector<DetectionResult> YOLOInference::preprocess_image(const cv::Mat& image) {
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(640, 640));

    cv::Mat preprocessed;
    cv::cvtColor(resized, preprocessed, cv::COLOR_BGR2RGB);
    
    return preprocess(preprocessed, 10.0f);
}

std::vector<DetectionResult> YOLOInference::infer(const cv::Mat& image) {
    cv::Mat preprocessed = preprocess_image(image);
    
    // Preprocess the image
    std::vector<DetectionResult> results = preprocess(preprocessed, 10.0f);
    
    // Postprocess the results
    return postprocess_results(results);
}

std::vector<DetectionResult> YOLOInference::postprocess_results(
    const std::vector<DetectionResult>& results) {
    std::vector<DetectionResult> detected;

    for (const auto& result : results) {
        if (result.confidence > conf_threshold_) {
            detected.push_back(result);
        }
    }

    detected = non_max_suppression(detected, iou_threshold);

    return detected;
}

} // namespace yolo
