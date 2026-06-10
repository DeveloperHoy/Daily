#include "ImageUtils.h"

cv::Mat ImageUtils::loadImage(const std::string& path) {
    // Check file exists
    std::ifstream file(path);
    if (!file.good()) {
        throw std::runtime_error("Image not found: " + path);
    }
    file.close();

    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Failed to load image: " + path);
    }

    return image;
}

cv::Mat ImageUtils::preprocessImage(cv::Mat& image, int width, int height) {
    // Resize if needed
    if (image.cols != width || image.rows != height) {
        cv::Mat resized;
        cv::resize(image, resized, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
        return resized;
    }

    // Convert to BGR to RGB
    cv::Mat rgb;
    cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);

    // Normalize to 0-255 to 0-1
    cv::Mat normalized;
    normalize(rgb, normalized, 0, 1, CV_32FC3);

    return normalized;
}

cv::Mat ImageUtils::restoreImage(cv::Mat& image, int originalWidth, int originalHeight) {
    cv::Mat restore = image.clone();
    cv::resize(restore, restore, cv::Size(originalWidth, originalHeight));
    return restore;
}

void ImageUtils::saveImage(const cv::Mat& image, const std::string& path) {
    cv::imwrite(path, image);
}

std::vector<cv::Point2f> ImageUtils::getCoordinates(cv::Mat& image) {
    std::vector<cv::Point2f> coordinates;
    
    // Simulate coordinate normalization logic
    // Convert normalized coordinates back to original image space
    for (size_t i = 0; i < image.size(); ++i) {
        for (size_t j = 0; j < image.size(); ++j) {
            coordinates.push_back(cv::Point2f(j, i));
        }
    }
    return coordinates;
}
