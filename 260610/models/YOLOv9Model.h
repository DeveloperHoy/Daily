#pragma once
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>
#include <memory>

class YOLOv9Model {
public:
    YOLOv9Model(const std::string& modelPath);
    ~YOLOv9Model();
    
    std::vector<float> getPrediction(const std::vector<float>& inputData);
    std::vector<Struct> detectObjects(cv::Mat& image, const std::vector<float>& predictions);
    
    std::vector<float> classThresholds;
    std::vector<std::pair<int, int>> anchorBoxes;

private:
    void validateModel();
    std::vector<std::pair<int, int>> loadAnchorBoxes();
    std::vector<std::string> loadClasses();
    void preprocessInputData(cv::Mat& image, std::vector<float>& inputData, int batchSize = 1);

private:
    Ort::Session session;
    Ort::SessionOptions sessionOptions;
    std::string modelPath;
    bool isValidModel;
};
