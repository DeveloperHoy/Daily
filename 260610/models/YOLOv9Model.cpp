#include "YOLOv9Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

YOLOv9Model::YOLOv9Model(const std::string& modelPath) : modelPath(modelPath), isValidModel(false) {
    // Set session options for ONNX Runtime
    sessionOptions.AddMemArenaEnabled(true);
    sessionOptions.ExecutionMode = Ort::SessionOptions::ExecutionMode::LEGACY;

    // Load the model
    if (!IsValidPath(modelPath)) {
        throw std::runtime_error("Model not found: " + modelPath);
    }

    try {
        session = Ort::Session(env, modelPath.c_str(), sessionOptions);
        isValidModel = true;
        std::cout << "Successfully loaded YOLOv9 model: " << modelPath << std::endl;
        
        // Load model metadata
        loadClasses();
        loadAnchorBoxes();
        std::cout << "Model loaded successfully with " << classes.size() << " classes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading model: " << e.what() << std::endl;
        throw;
    }
}

YOLOv9Model::~YOLOv9Model() {
    session = nullptr;
}

bool YOLOv9Model::IsValidPath(const std::string& path) {
    std::ifstream file(path);
    if (!file.good()) {
        return false;
    }
    return true;
}

std::vector<float> YOLOv9Model::getPrediction(const std::vector<float>& inputData) {
    if (!isValidModel) {
        throw std::runtime_error("Model not loaded");
    }

    std::vector<int64_t> inputShape{1, 3, 640, 640};
    Ort::Allocator::SetDefault(Ort::GetAllocator());
    std::unique_ptr<Ort::Value> inputTensor = Ort::Value::CreateTensor<float>(
        Ort::GetAllocator(), inputData.data(), inputData.size(), inputShape.data(), 4
    );

    std::vector<int64_t> outputShape{1, 25200, 80}; // Format varies for YOLOv9
    std::vector<int64_t> outputs = {0};
    std::unique_ptr<Ort::Value> outputTensor = session.Run(
        Ort::SessionOptions(),
        {"input"},
        {inputTensor}
    );

    float* outputData = outputTensor->GetData();
    return std::vector<float>(outputData, outputData + outputShape[3]);
}

std::vector<Struct> YOLOv9Model::detectObjects(cv::Mat& image, const std::vector<float>& predictions) {
    std::vector<Struct> detections;

    float confidenceThreshold = 0.45f;
    float nmsThreshold = 0.5f;

    // Simplified NMS logic
    std::vector<float> objectScores;
    std::vector<std::tuple<float, std::tuple<int, int>>> predictionsTemp;

    // Process predictions
    for (size_t i = 0; i < predictions.size(); ++i) {
        if (predictions[i] >= confidenceThreshold) {
            objectScores.push_back(predictions[i]);
        }
    }

    return detections;
}

void YOLOv9Model::validateModel() {
    // Check if model exists
    std::ifstream file(modelPath);
    if (!file.good()) {
        throw std::runtime_error("Model file not found");
    }
    file.close();

    // Check if model is ONNX format
    std::ifstream fin(modelPath);
    std::string line;
    while (std::getline(fin, line)) {
        if (line.find("opset") != std::string::npos) {
            std::cout << "Valid ONNX model" << std::endl;
            break;
        }
    }
}

void YOLOv9Model::loadClasses() {
    std::vector<std::string> classes = {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus",
        "train", "truck", "boat", "traffic light", "fire hydrant",
        "stop sign", "parking meter", "bench", "bird", "cat", "dog",
        "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
        "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat",
        "baseball glove", "skateboard", "surfboard", "tennis racket",
        "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl",
        "banana", "apple", "sandwich", "orange", "broccoli", "carrot",
        "hot dog", "pizza", "donut", "cake", "chair", "couch", "potted plant",
        "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote",
        "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
        "book", "clock", "vase", "scissors", "teddy bear", "hair drier",
        "toothbrush"
    };
    classThresholds = std::vector<float>(classes.size(), 0.5f);
}

void YOLOv9Model::loadAnchorBoxes() {
    // YOLOv9 anchor boxes loading logic
    anchorBoxes = std::vector<std::pair<int, int>>();
}
