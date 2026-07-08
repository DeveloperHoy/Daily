#pragma once
#include <memory>
#include <vector>
#include "Tracker.h"

namespace ai::tracking {

class ByteTrackEngine {
public:
    ByteTrackEngine(int trackingThreads = std::thread::hardware_concurrency());
    ~ByteTrackEngine();

    [[nodiscard]] std::vector<Track> predict(const std::vector<std::vector<DetectedObject>>& detections);
    [[nodiscard]] std::vector<Track> update(const std::vector<std::vector<DetectedObject>>& detections, const std::vector<Track>& tracks);

    void initialize();
    void reset();

    void setDetectionClassifier(const std::string& classifier);
    [[nodiscard]] bool isInitialized() const;

    [[nodiscard]] float getDetectionThreshold() const;
    void setDetectionThreshold(float threshold);

    [[nodiscard]] float getMatchingThreshold() const;
    void setMatchingThreshold(float threshold);

    void processFrame(const std::vector<DetectedObject>& detections, std::vector<Track>& tracks);

private:
    [[nodiscard]] std::vector<Track> initializeTracking(
        const std::vector<DetectedObject>& detections);

    [[nodiscard]] std::vector<Track> matchDetectionToTracks(
        const std::vector<DetectedObject>& detections,
        const std::vector<Track>& tracks);

    [[nodiscard]] std::vector<Track> matchLostTracks(
        const std::vector<Track>& tracks);

    [[nodiscard]] float calculateCostMatrix(
        const std::vector<DetectedObject>& detections,
        const std::vector<Track>& tracks);

    [[nodiscard]] float calculateBoxOverlapMetric(
        const std::vector<DetectedObject>& detection,
        const std::vector<Track>& tracks);

    [[nodiscard]] float calculateAppearanceMetric(const DetectionObject& detection, const Track& track);

private:
    std::shared_ptr<utils::ThreadPool> m_threadPool;
    float m_detectionThreshold = 0.6f;
    float m_matchingThreshold = 0.8f;
    std::vector<float> m_detectionWeights = {1.0f, 0.5f, 0.5f};
    std::vector<float> m_matchingWeights = {0.8f, 1.0f, 0.5f};
};

} // namespace ai::tracking
