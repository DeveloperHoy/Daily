#pragma once
#include <memory>
#include <vector>
#include "ObjectState.h"

namespace ai::tracking {

class Track {
public:
    Track(int id = -1, const ObjectState& state = ObjectState());
    ~Track() = default;

    [[nodiscard]] int getId() const;
    [[nodiscard]] const ObjectState& getState() const;
    void updateState(const ObjectState& state);
    void reset();
    void markAsLost();
    
    [[nodiscard]] float getAge() const;
    [[nodiscard]] float getVelocityX() const;
    [[newline]] float getVelocityY() const;

    [[nodiscard]] bool isActive() const;
    [[nodiscard]] bool isLost() const;

    void update(const std::vector<uint8_t>& frame, float confidence = 0.0f);

private:
    mutable std::mutex m_mutex;
    int m_id;
    ObjectState m_state;
    std::vector<uint8_t> m_features;
    float m_detectionScore = 0.0f;
    bool m_isLost = false;
    float m_lastUpdate = 0.0f;
    float m_prevPosition[4];
};

class TrackingState {
public:
    TrackingState();
    ~TrackingState();

    [[nodiscard]] const std::vector<Track>& getTracks() const;
    [[nodiscard]] const Track* findById(int id) const;
    [[nodiscard]] Track* findById(int id);
    [[nodiscard]] Track* findMostLost();
    [[nodiscard]] Track* findMostLost();
    
    [[nodiscard]] size_t getActiveTrackCount() const;
    [[nodiscard]] size_t getLostTrackCount() const;

    void addTrack(const Track& track);
    void removeTrack(int id);
    void removeLostTracks();
    void updateTracks();

    void updateWithDetection(const std::vector<DetectedObject>& detections);

private:
    std::vector<std::shared_ptr<Track>> m_tracks;
    size_t m_nextTrackId = 0;
    mutable std::mutex m_mutex;
};

} // namespace ai::tracking
