#pragma once

#include <memory>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "../model/yolov9_model.h"
#include "../output/detection_result.h"

namespace yolo {

class YOLOInference {
public:
    YOLOInference(std::unique_ptr<YOLOv9Model> model);
    ~YOLOInference() = default;

    std::vector<DetectionResult> infer(const cv::Mat& image);

    void set_conf_threshold(float threshold) { conf_threshold_ = threshold; }
    float get_conf_threshold() const { return conf_threshold_; }

    void set_iou_threshold(float threshold) { iou_threshold_ = threshold; }
    float get_iou_threshold() const { return iou_threshold_; }

    void set_output_dir(const std::string& dir) { output_dir_ = dir; }

private:
    std::vector<DetectionResult> preprocess_image(const cv::Mat& image);
    std::vector<DetectionResult> postprocess_results(const std::vector<DetectionResult>& results);
    float calculate_iou(const DetectionResult& a, const DetectionResult& b);
    std::vector<DetectionResult> non_max_suppression(
        std::vector<DetectionResult>& results, float iou_threshold);
    
    float conf_threshold_ = 0.5f;
    float iou_threshold_ = 0.5f;
    std::string output_dir_ = "./output";
    std::unique_ptr<YOLOv9Model> model_;
};

} // namespace yolo
