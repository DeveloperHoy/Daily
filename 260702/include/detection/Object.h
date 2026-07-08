#pragma once
#include <memory>
#include <vector>
#include <string>

namespace ai::detection {

struct BoundingBox {
    int classId = -1;
    float confidence = 0.0f;
    float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
    
    [[nodiscard]] float getWidth() const { return x2 - x1; }
    [[nodiscard]] float getHeight() const { return y2 - y1; }
    [[nodiscard]] float getCenterX() const { return (x1 + x2) / 2.0f; }
    [[nodiscard]] float getCenterY() const { return (y1 + y2) / 2.0f; }
    [[nodiscard]] float getArea() const { return getWidth() * getHeight(); }
};

struct DetectedObject {
    int id = -1;
    std::string className;
    BoundingBox bbox;
    float confidence;
    float nmsThreshold = 0.5f;
    float score = 0.0f;
    std::vector<float> extraData; // For tracking, features, etc.
    
    [[nodiscard]] const std::string& getClassName() const { return className; }
    [[nodiscard]] float getConfidence() const { return confidence; }
    
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool operator<(const DetectedObject&) const;
    [[nodiscard]] bool operator>(const DetectedObject&) const;
    [[nodiscard]] bool operator==(const DetectedObject&) const;
};

class Detector {
public:
    enum class DetectionType {
        YOLOV9,
        YOLOV5,
        YOLOV8,
        SSD,
        CUSTOM
    };

    Detector(const std::string& modelPath, 
             const std::string& weightsPath,
             int numClasses,
             DetectionType type,
             float confidenceThreshold = 0.5f,
             float nmsThreshold = 0.45f,
             int batchSize = 1,
             int imgWidth = 640,
             int imgHeight = 480);
    
    ~Detector();

    [[nodiscard]] const std::string& getModelPath() const;
    void setModelPath(const std::string& path);

    [[nodiscard]] bool isInitialized() const;
    bool initialize();
    void shutdown();

    [[nodiscard]] std::vector<std::vector<DetectedObject>> detect(
        const std::vector<std::vector<uint8_t>>& frames,
        const std::vector<std::vector<int>>& classIds = std::vector<int>{});

    [[nodiscard]] std::vector<DetectedObject> detectSingle(
        const std::vector<uint8_t>& frame,
        const std::vector<std::vector<int>>& classIds = std::vector<int>{});

    [[nodiscard]] std::vector<DetectedObject>> detectStream(
        const std::vector<std::vector<uint8_t>>& frames,
        size_t maxFrames = std::numeric_limits<size_t>::max());

    [[nodiscard]] float getInferenceTime() const;
    void setInferenceTime(float time);

    [[nodiscard]] size_t getBatchSize() const;
    [[nodiscard]] void setBatchSize(size_t size);

    [[nodiscard]] const ClassManager& getClassManager() const;
    [[nodiscard]] float getConfidenceThreshold() const;
    void setConfidenceThreshold(float threshold);
    
    [[nodiscard]] float getNmsThreshold() const;
    void setNmsThreshold(float threshold);

    [[nodiscard]] float getIoUThreshold() const;
    void setIoUThreshold(float threshold);

    [[nodiscard]] size_t getInferenceCount() const;
    [[nodiscard]] float getAverageInferenceTime() const;

    void postProcessOutput(float* outputData, size_t size);
    void preprocessInput(std::vector<uint8_t>& input, int width, int height);
    std::vector<uint8_t> postprocessOutput();

private:
    [[nodiscard]] std::vector<std::vector<float>> preprocessImages(
        const std::vector<std::vector<uint8_t>>& images);
    
    [[nodiscard]] std::vector<DetectedObject> decodeDetection(
        const std::vector<DetectedObject>& detections);

    std::string m_modelPath;
    std::string m_weightsPath;
    int m_numClasses;
    DetectionType m_type;
    float m_confidenceThreshold;
    float m_nmsThreshold;
    int m_batchSize;
    int m_imgWidth;
    int m_imgHeight;
    
    std::shared_ptr<ClassManager> m_classManager;
    utils::Timer m_inferenceTimer;
    size_t m_inferenceCount = 0;
    utils::Timer m_totalTimer;
    bool m_isInitialized = false;
};

} // namespace ai::detection
