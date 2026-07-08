#pragma once
#include <memory>
#include <vector>
#include <array>

namespace ai::tracking {

// Track state information
class ObjectState {
public:
    ObjectState() = default;
    
    void set(int id, float x1, float y1, float x2, float y2);
    
    [[nodiscard]] int getId() const { return m_id; }
    [[nodiscard]] float getX1() const { return m_x1; }
    [[nodiscard]] float getY1() const { return m_y1; }
    [[nodiscard]] float getX2() const { return m_x2; }
    [[nodiscard]] float getY2() const { return m_y2; }
    [[nodiscard]] float getCenterX() const { return (m_x1 + m_x2) / 2.0f; }
    [[nodiscard]] float getCenterY() const { return (m_y1 + m_y2) / 2.0f; }
    [[nodiscard]] float getVelocityX() const { return m_velocityX; }
    [[nodiscard]] float getVelocityY() const { return m_velocityY; }
    [[nodiscard]] float getScore() const { return m_score; }
    [[nodiscard]] float getAge() const { return m_age; }
    
    [[nodiscard]] std::vector<float> getFeatureVector() const {
        return m_feature;
    }
    
    [[nodiscard]] bool isAlive() const { return m_age < m_maxAge; }
    [[nodiscard]] bool isLost() const { return !m_isLost; }
    [[nodiscard]] bool isActive() const { 
        return m_id > 0 && !m_isLost && m_age < m_maxAge; 
    }

    void update(int id, float x1, float y1, float x2, float y2, 
               float score, float velocityX, float velocityY,
               float age = 0.0f);
    void markLost();
    void updateAge();

    [[nodiscard]] float getIoU(const ObjectState&) const;
    [[nodiscard]] float getCentroidDistance(const ObjectState&) const;
    [[nodiscard]] float calculateMetrics() const;
    void reset();

    [[nodiscard]] ObjectState& clone();

private:
    mutable std::mutex m_mutex;
    int m_id = 0;
    float m_x1 = 0.0f;
    float m_y1 = 0.0f;
    float m_x2 = 0.0f;
    float m_y2 = 0.0f;
    float m_velocityX = 0.0f;
    float m_velocityY = 0.0f;
    float m_score = 0.0f;
    float m_age = 0.0f;
    float m_maxAge = 30.0f;
    std::vector<float> m_feature;
    bool m_isLost = false;
    bool m_valid = false;
};

inline ObjectState::ObjectState() = default;

inline void ObjectState::set(int id, float x1, float y1, float x2, float y2) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_id = id;
    m_x1 = x1; m_y1 = y1;
    m_x2 = x2; m_y2 = y2;
    m_valid = true;
    m_age = 0.0f;
}

inline void ObjectState::update(int id, float x1, float y1, float x2, float y2,
                               float score, float velocityX, float velocityY,
                               float age) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_id = id;
    m_x1 = x1; m_y1 = y1;
    m_x2 = x2; m_y2 = y2;
    m_score = score;
    m_velocityX = velocityX;
    m_velocityY = velocityY;
    m_age = age;
    m_isLost = false;
}

inline void ObjectState::markLost() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_isLost = true;
}

inline void ObjectState::updateAge() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_age++;
}

inline float ObjectState::getIoU(const ObjectState& other) const {
    // Intersection over Union calculation
    // Implementation
    return 0.0f;
}

inline float ObjectState::getCentroidDistance(const ObjectState& other) const {
    float dx = (m_x2 + m_x1) / 2.0f - other.m_x2;
    float dy = (m_y2 + m_y1) / 2.0f - other.m_y2;
    return std::sqrt(dx * dx + dy * dy);
}

inline float ObjectState::calculateMetrics() const {
    return (m_age / m_maxAge);
}

inline ObjectState ObjectState::clone() const {
    ObjectState* clone = new ObjectState();
    *clone = *this;
    return clone;
}

} // namespace ai::tracking
