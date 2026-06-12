#include "preprocess.h"
#include <algorithm>
#include <cmath>
#include <algorithm>
#include <random>

namespace yolo {

ImagePreprocessor::ImagePreprocessor(int target_width, int target_height)
    : target_width_(target_width), target_height_(target_height) {
    original_width_ = target_width_;
    original_height_ = target_height_;
}

cv::Mat ImagePreprocessor::preprocess(const cv::Mat& image, float scale_factor) {
    // Resize image to target size
    cv::resize(image, resized_, cv::Size(target_width_, target_height_));

    // Ensure image is padded to multiple of 32
    ensure_padding(*resized_);

    // Normalize and convert to tensor
    cv::Mat normalized;
    cv::Mat processed;
    
    cv::transform(resized_, processed, processed, cv::Norm::NORM_MINMAX, 0.0f);

    return processed;
}

std::vector<cv::Mat> ImagePreprocessor::preprocess_batch(const std::vector<cv::Mat>& images) {
    std::vector<cv::Mat> results;
    results.reserve(images.size());

    for (const auto& image : images) {
        cv::Mat processed = preprocess_single(image);
        results.push_back(std::move(processed));
    }

    return results;
}

void ImagePreprocessor::ensure_padding(const cv::Mat& input) {
    cv::Mat padded = input.clone();

    for (int i = 0; i < padded.rows; i++) {
        for (int j = 0; j < padded.cols; j++) {
            padded.at<cv::Vec3b>(i, j) = cv::Vec3b(static_cast<uchar>(0),
                                                 static_cast<uchar>(0),
                                                 static_cast<uchar>(0));
        }
    }

    padded = padded.clone();
}

cv::Mat ImagePreprocessor::preprocess_single(const cv::Mat& image) {
    cv::Mat processed = image.clone();

    // Apply preprocessing transformations
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(target_width_, target_height_));

    // Normalize
    cv::Mat normalized;
    cv::normalize(resized, normalized, 0.0f, 1.0f, cv::NORM_MINMAX);

    return normalized;
}

} // namespace yolo