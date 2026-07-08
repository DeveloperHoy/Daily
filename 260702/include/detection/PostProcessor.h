#pragma once
#include <vector>
#include "Object.h"

namespace ai::detection {

// Non-Maximum Suppression
class NonMaxSuppression {
public:
    NonMaxSuppression(float confidenceThreshold = 0.5f, float nmsThreshold = 0.45f);
    ~NonMaxSuppression() = default;

    [[nodiscard]] std::vector<DetectedObject> nonMaxSuppression(
        const std::vector<DetectedObject>& detections);

    [[nodiscard]] std::vector<DetectedObject> filterByClass(
        const std::vector<DetectedObject>& detections,
        const std::vector<int>& classIds);

    [[nodiscard]] std::vector<DetectedObject] filterByConfidence(
        const std::vector<DetectedObject>& detections,
        float confidenceThreshold);

private:
    [[nodiscard]] std::vector<size_t> getSortedIndices(
        const std::vector<DetectedObject>& detections);

    [[nodiscard]] std::vector<DetectedObject> filterDetections(
        const std::vector<DetectedObject>& detections,
        const std::vector<size_t>& indices);

    float m_confidenceThreshold = 0.5f;
    float m_nmsThreshold = 0.45f;
};

// Box filtering and optimization
class PostProcessor {
public:
    PostProcessor(
        float confidenceThreshold = 0.5f,
        float nmsThreshold = 0.45f,
        float iouThreshold = 0.45f,
        size_t maxDetections = 100);

    [[nodiscard]] std::vector<DetectedObject> process(
        const std::vector<DetectedObject>& detections);

    [[nodiscard]] std::vector<DetectedObject> processFrames(
        const std::vector<std::vector<DetectedObject>>& frameDetections);

    [[nodiscard]] std::vector<DetectedObject> filter(
        const std::vector<DetectedObject>& detections,
        float confidenceThreshold,
        float nmsThreshold,
        float iouThreshold = 0.45f,
        size_t maxDetections = 100);

    [[nodiscard]] std::vector<DetectedObject> applyCustomFilters(
        const std::vector<DetectedObject>& detections);

private:
    float m_confidenceThreshold = 0.5f;
    float m_nmsThreshold = 0.45f;
    float m_iouThreshold = 0.45f;
    size_t m_maxDetections = 100;
    NonMaxSuppression m_nms;
};

} // namespace ai::detection
