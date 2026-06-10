#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <stdexcept>
#include <vector>

class ImageUtils {
public:
    static cv::Mat loadImage(const std::string& path);
    static cv::Mat preprocessImage(cv::Mat& image, int width = 640, int height = 640);
    static cv::Mat restoreImage(cv::Mat& image, int originalWidth, int originalHeight);
    static void saveImage(const cv::Mat& image, const std::string& path);
    static std::vector<cv::Point2f> getCoordinates(cv::Mat& image);

private:
    static bool validateImage(const std::string& path);
    static bool validateImageFormat(const std::string& path);
    static std::string trimWhitespace(const std::string& str);
};
