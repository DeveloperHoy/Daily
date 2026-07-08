#pragma once
#include "Detector.h"
#include "ONNXRuntime/onnxruntime_cxx_api.h"

namespace ai::detection {

class ONNXDetector {
public:
    ONNXDetector(const std::string& modelPath,
                 const std::string& providers = "CUDAExecutionProvider",
                 float sessionOptions = 0.0f);
    
    ~ONNXDetector();

    [[nodiscard]] bool loadModel();
    [[nodiscard]] bool isModelLoaded() const;
    
    [[nodiscard]] std::vector<uint8_t] runInference(
        const std::vector<uint8_t>& input);
    
    [[nodiscard]] std::vector<float> getOutput() const;
    
    [[nodiscard]] float inferTime() const;
    void setInferTime(float time);

private:
    [[nodiscard]] ONNEXRuntime::Session m_session;
    [[nodiscard]] void setInput();
    [[nodiscard]] void* allocateMemory(size_t size);
    [[nodiscard]] void deallocateMemory(void* ptr);
    [[nodiscard]] void* allocateMemory(size_t size);
    void releaseMemory();
};

} // namespace ai::detection
