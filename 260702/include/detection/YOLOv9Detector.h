#pragma once
#include "Detector.h"
#include <vector>
#include <array>

namespace ai::detection {

// YOLOv9 Model Configuration
class YOLOv9Config {
public:
    enum class ModelSize {
        S = 320,  // Small
        M = 640,  // Medium
        L = 1280  // Large
    };

    enum class ModelArchitecture {
        V1 = 1,  // Original YOLOv9
        V2 = 2  // YOLOv9 v2
    };

    YOLOv9Config(ModelSize size, ModelArchitecture arch = ModelArchitecture::V1);
    
    [[nodiscard]] int getAnchorSize() const;
    [[nodiscard]] int getOutputChannels() const;
    [[nodiscard]] int getImageSize() const;
    [[nodiscard]] int getNumOutput() const;
    [[nodiscard]] float getMosaicScale() const;
    [[nodiscard]] float getLambd() const;
    [[nodiscard]] float getHsvMixRate() const;
    
    void setAnchorSize(int size);
    void setModelSize(ModelSize size);
    void setArchitecture(ModelArchitecture arch);

private:
    ModelSize m_size = ModelSize::S;
    ModelArchitecture m_arch = ModelArchitecture::V1;
    int m_anchorSize = 640;
    int m_outputChannels = {19, 10, 85};
    int m_numOutput = {4, 4, 4};
    float m_mosaicScale = 1.0f;
    float m_lambd = 0.5f;
    float m_hsvMixRate = 0.5f;
};

// YOLOv9 Detector Implementation
class YOLOv9Detector final : public Detector {
public:
    YOLOv9Detector(const std::string& modelPath,
                   const std::string& weightsPath,
                   int numClasses,
                   float confidenceThreshold = 0.5f,
                   float nmsThreshold = 0.45f,
                   int batchSize = 1,
                   int imgWidth = 640,
                   int imgHeight = 480);
    
    ~YOLOv9Detector();

    [[nodiscard]] bool initialize() override;
    [[nodiscard]] std::vector<DetectedObject> detectStream(
        const std::vector<std::vector<uint8_t>>& frames,
        size_t maxFrames = std::numeric_limits<size_t>::max()) override;

    [[nodiscard]] std::vector<DetectedObject>> detectStreamWithCache(
        const std::vector<std::vector<uint8_t>>& frames,
        size_t maxFrames = std::numeric_limits<size_t>::max(),
        float memoryBudget = 10000.0f);

    [[nodiscard]] float calculateModelMetric() const;
    [[nodiscard]] void printModelMetrics();

private:
    // ONNX Runtime initialization
    [[nodiscard]] ONNXRuntime::Session& getSession();
    [[nodiscard]] bool validateModel(const std::string& modelPath);
    
    // Feature extraction
    [[nodiscard]] std::vector<std::vector<float>> extractFeatures(
        const std::vector<DetectedObject>& objects);

    [[nodiscard]] std::vector<utils::Point2f> calculateAnchorBoxes() const;
    [[nodiscard]] std::vector<std::vector<float>> generateAnchors() const;

    [[nodiscard]] std::vector<float] forward(const std::vector<std::vector<uint8_t>>& frames);

    std::shared_ptr<ONNXRuntime::Session> m_session;
    YOLOv9Config m_config;
    std::vector<utils::Point2f> m_anchors;
};

} // namespace ai::detection
