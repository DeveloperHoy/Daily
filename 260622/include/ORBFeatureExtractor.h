#pragma once
#include "IFeatureExtractor.h"
#include <opencv2/features2d.hpp>

/**
 * @brief Implements IFeatureExtractor using ORB features.
 */
class ORBFeatureExtractor : public IFeatureExtractor {
public:
    ORBFeatureExtractor();
    ~ORBFeatureExtractor() override = default;

    std::pair<std::vector<cv::KeyPoint>, cv::Mat> extract(const cv::Mat& input) const override;
    float compare(const cv::Mat& test_descriptors, 
                  const std::vector<cv::KeyPoint>& template_keypoints, 
                  const cv::Mat& template_descriptors) const override;

    size_t getFeatureDimension() const override { return ORB_DESCRIPTOR_SIZE; }

private:
    cv::Ptr<cv::ORB> orb_;
};
