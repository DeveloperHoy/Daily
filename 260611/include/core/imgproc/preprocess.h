#pragma once

#include <memory>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "../../utils/logger.h"

namespace yolo {

class ImagePreprocessor {
public:
    ImagePreprocessor(int target_width = 640, int target_height = 640);
    ~ImagePreprocessor() = default;

    cv::Mat preprocess(const cv::Mat& image, float scale_factor = 1.0f);
    std::vector<cv::Mat> preprocess_batch(const std::vector<cv::Mat>& images);

    void set_target_size(int width, int height) { target_width_ = width; target_height_ = height; }
    int get_target_width() const { return target_width_; }
    int get_target_height() const { return target_height_; }

private:
    int target_width_ = 640;
    int target_height_ = 640;
    int original_width_ = 640;
    int original_height_ = 640;
    int img_channels_ = 3;
    int img_depth_ = 3;

    std::unique_ptr<cv::Mat> resized_;
    std::unique_ptr<cv::Mat> padded_;
    std::unique_ptr<cv::Mat> processed_;
    std::vector<cv::Mat> processed_images_;

    cv::Mat preprocess_single(const cv::Mat& image);
    void ensure_padding(const cv::Mat& input);
};

} // namespace yolo
