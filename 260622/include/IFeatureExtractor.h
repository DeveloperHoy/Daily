#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

/**
 * @brief Defines the interface for all feature extraction methods.
 * Enables swapping out algorithms (e.g., ORB, CLIP) without changing PatternDatabase logic.
 */
class IFeatureExtractor {
public:
    virtual ~IFeatureExtractor() = default;

    /**
     * @brief Extracts features (keypoints and descriptors) from an image segment.
     * @param input The source OpenCV Mat object.
     * @return A pair containing keypoints and a vector of descriptors.
     */
    virtual std::pair<std::vector<cv::KeyPoint>, cv::Mat> extract(const cv::Mat& input) const = 0;

    /**
     * @brief Compares the feature set of a new patch against an existing template.
     * @param test_descriptors Descriptors from the newly detected pattern.
     * @param template_keypoints Keypoints from the stored template.
     * @param template_descriptors Descriptors from the stored template.
     * @return A similarity score (float) based on matches found.
     */
    virtual float compare(const cv::Mat& test_descriptors, 
                           const std::vector<cv::KeyPoint>& template_keypoints, 
                           const cv::Mat& template_descriptors) const = 0;

    /**
     * @brief Calculates the required feature vector dimension for initialization/pre-sizing.
     */
    virtual size_t getFeatureDimension() const = 0;
};
