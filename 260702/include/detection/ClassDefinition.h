#pragma once
#include <string>
#include <vector>
#include <numeric>

namespace ai::detection {

class ObjectClass {
public:
    ObjectClass(const std::string& name, float confidenceThreshold = 0.5f);
    ~ObjectClass() = default;

    [[nodiscard]] std::string getName() const { return m_name; }
    void setName(const std::string& name);
    
    [[nodiscard]] float getThreshold() const { return m_threshold; }
    void setThreshold(float threshold);
    
    [[nodiscard]] const std::vector<int>& getClassIds() const { return m_classIds; }
    void addClassId(int id);
    void setClassIds(const std::vector<int>& ids);
    
    [[nodiscard]] size_t getClassCount() const { return m_classIds.size(); }
    [[nodiscard]] float calculateIoU(const ObjectClass&) const;
    
    [[nodiscard]] bool matches(const std::string& name) const;
    [[nodiscard]] bool hasThreshold() const { return !m_threshold; }
    void setConfidenceThreshold(float t);
    void setNMSThreshold(float t);

    void validate();

private:
    std::string m_name;
    float m_threshold = 0.5f;
    float m_iouThreshold = 0.45f;
    float m_classIou = 0.5f;
    std::vector<int> m_classIds;
    float m_prevalence = 0.0f;
};

inline ObjectClass::ObjectClass(const std::string& name, float confidenceThreshold)
    : m_name(name), m_threshold(confidenceThreshold) {
    validate();
}

inline void ObjectClass::setName(const std::string& name) {
    m_name = name;
}

inline void ObjectClass::addClassId(int id) {
    m_classIds.push_back(id);
    validate();
}

inline void ObjectClass::setClassIds(const std::vector<int>& ids) {
    m_classIds = ids;
    validate();
}

inline void ObjectClass::setConfidenceThreshold(float t) {
    m_threshold = t;
}

inline void ObjectClass::setNMSThreshold(float t) {
    m_iouThreshold = t;
}

inline void ObjectClass::validate() {
    if (m_name.empty()) {
        m_name = "Unknown";
    }
    if (m_threshold < 0.0f || m_threshold > 1.0f) {
        m_threshold = 0.5f;
    }
}

inline bool ObjectClass::matches(const std::string& name) const {
    return m_name == name;
}

class ClassManager {
public:
    ClassManager();
    ~ClassManager();

    [[nodiscard]] const std::vector<ObjectClass>& getClassDefinitions() const;
    void addClass(const ObjectClass& cls);
    bool hasClass(const std::string& name) const;
    
    [[nodiscard]] size_t getClassCount() const;
    
    void loadClasses(const std::string& filepath);
    void saveClasses(const std::string& filepath) const;
    
    [[nodiscard]] const std::vector<std::string>& getNames() const;
    [[nodiscard]] const std::vector<float>& getThresholds() const;
    [[nodiscard]] float getThreshold(const size_t index) const;
    void setThreshold(const size_t index, float value);

private:
    void parseClassYoloFormat(const std::vector<std::string>& names, 
                              const std::vector<float>& thresholds);
    void registerClass(const std::string& name, float threshold);

    std::vector<ObjectClass> m_classes;
    mutable std::mutex m_mutex;
    std::string m_classPath;
};

} // namespace ai::detection
