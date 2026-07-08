#pragma once
#include <memory>
#include <vector>
#include "Track.h"
#include "ByteTrackEngine.h"

namespace ai::tracking {

class Tracker {
public:
    enum class TrackingMode {
        BYTE_TRACK,
        SORT,
        DeepSORT,
        CUSTOM
    };

    Tracker(
        TrackingMode mode = TrackingMode::BYTE_TRACK,
        int maxAge = 30,
        float detectionThreshold = 0.6f,
        float trackingThreshold = 0.8f,
        size_t maxTracks = 100
    );
    
    ~Tracker();

    [[nodiscard]] std::vector<Track> trackObjects(
        const std::vector<DetectedObject>& objects) const;

    [[nodiscard]] std::vector<Track> update(
        const std::vector<std::vector<DetectedObject>>& frameDetections) const;

    [[nodiscard]] std::vector<Track> startSession(
        const std::vector<std::vector<DetectedObject>>& initialDetections) const;

    [[nodiscard]] void addManualTrack(int id, float x, float y, float w, float h);

    [[nodiscard]] bool isTracking(int id) const;
    [[nodiscard]] bool isLost(int id) const;
    
    [[nodiscard]] size_t getActiveTrackCount() const;
    [[nodiscard]] size_t getTotalTrackCount() const;

    void setDetectionThreshold(float threshold);
    void setTrackingThreshold(float threshold);
    void setMaxAge(float age);
    void setMaxTracks(size_t max);
    
    [[nodiscard]] TrackingMode getMode() const;
    void setMode(TrackingMode mode);

private:
    TrackingMode m_mode = TrackingMode::BYTE_TRACK;
    float m_detectionThreshold = 0.6f;
    float m_trackingThreshold = 0.8f;
    float m_maxAge = 30.0f;
    size_t m_maxTracks = 100;
    
    std::shared_ptr<ByteTrackEngine> m_engine;
};

} // namespace ai::tracking
