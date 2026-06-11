#include "yolov9_model.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace yolo {

// Default YOLOv9 model configuration
namespace {
    std::vector<std::pair<int, int>> get_default_anchors() {
        std::vector<std::pair<int, int>> anchors;
        // YOLOv9 default anchor boxes (in pixels)
        anchors.push_back({10, 13});
        anchors.push_back({16, 30});
        anchors.push_back({33, 23});
        anchors.push_back({30, 61});
        anchors.push_back({62, 45});
        anchors.push_back({19, 36});
        anchors.push_back({35, 45});
        anchors.push_back({59, 113});
        anchors.push_back({116, 90});
        anchors.push_back({156, 198});
        return anchors;
    }
    
    std::vector<std::pair<int, int>> get_anchors_for_scale(float scale) {
        std::vector<std::pair<int, int>> anchors = get_default_anchors();
        // Scale anchors by input scale
        std::vector<std::pair<int, int>> scaled_anchors;
        for (auto& anchor : anchors) {
            scaled_anchors.push_back({static_cast<int>(anchor.first * scale), 
                                     static_cast<int>(anchor.second * scale)});
        }
        return scaled_anchors;
    }
}

YOLOv9Model::ModelHandle::ModelHandle(const std::string& model_path, const ModelConfig& config, 
                                     bool use_cpu, Ort::LoggingLevel log_level) 
    : path_(model_path), config_(config), use_cpu_(use_cpu) {
    config_ = config;
    
    // Create session
    this->session_ = std::make_unique<Ort::SessionHandle>();
    this->session_->Initialize(model_path.c_str());
    this->session_->Setup();
    
    // Load model config
    this->config_ = config;
    this->validate_model();
    this->init_start_ = std::chrono::steady_clock::now();
    this->init_end_ = std::chrono::steady_clock::now();
}

YOLOv9Model::ModelHandle::~ModelHandle() {
    this->session_.reset();
}

void YOLOv9Model::ModelHandle::validate_model() {
    if (config_.input_layer_name == "") {
        config_.input_layer_name = "input";
    }
    
    // Validate input layer
    auto inputs = session_->GetInputs();
    if (inputs.empty()) {
        throw std::runtime_error("Model has no input layer");
    }
    
    std::vector<std::pair<std::string, int>> input_map;
    for (size_t i = 0; i < inputs.size(); ++i) {
        std::string name = inputs[i].Name();
        if (name == config_.input_layer_name) {
            input_map.push_back({name, static_cast<int>(inputs[i].Elements())});
        }
    }
}

Ort::Value YOLOv9Model::ModelHandle::infer(const std::vector<float>& input) {
    std::vector<int64_t> input_shape{1, 3, 640, 640}; // Default for YOLOv9
    
    // Create input tensor
    auto input_tensor = std::make_unique<Ort::Value>(Ort::MemoryInfo::CreateCpu(), input.data(), input.size(), input_shape);
    
    // Execute
    std::vector<Ort::Value> output = session_->Run(input_tensor);
    
    return output[0];
}

void YOLOv9Model::ModelHandle::infer_batch(const std::vector<std::vector<float>>& inputs) {
    std::vector<int64_t> input_shape{static_cast<int64_t>(inputs.size()), 3, 640, 640};
    
    // Batch inference
    for (size_t i = 0; i < inputs.size(); ++i) {
        auto input_tensor = std::make_unique<Ort::Value>(Ort::MemoryInfo::CreateCpu(), inputs[i].data(), inputs[i].size(), input_shape);
        
        // Execute
        std::vector<Ort::Value> output = session_->Run(input_tensor);
        
        // Store output
        cached_outputs_.push_back(output[0]);
    }
}

YOLOv9Model::YOLOv9Model(const std::string& model_path, const ModelConfig& config) {
    this->config_ = config;
    this->initialized_ = true;
    
    // Load and initialize session
    load_model();
}

bool YOLOv9Model::validate_model(const std::string& path, const std::string& expected_ext) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string content;
    while (std::getline(file, line)) {
        content += line;
    }
    
    return content.find("opset") != std::string::npos ||
           content.find("model") != std::string::npos;
}

void YOLOv9Model::load_model() {
    load_start_ = std::chrono::steady_clock::now();
    
    try {
        session_ = std::make_unique<Ort::Session>(env_, model_path_, session_options_);
        session_handle_ = std::move(*session_);
        load_end_ = std::chrono::steady_clock::now();
        
        std::chrono::milliseconds load_time = 
            std::chrono::duration_cast<std::chrono::milliseconds>(load_end_ - load_start_);
        yolo::logging::Logger::info("Model loaded in {} ms", load_time.count());
        
        // Validate session
        valid_ = true;
        
    } catch (const std::exception& e) {
        yolo::logging::error("Failed to load model: {}", e.what());
        throw;
    }
}

void YOLOv9Model::validate_model() {
    std::ifstream file(model_path_);
    if (!file.is_open()) {
        throw std::runtime_error("Model file not found: " + model_path_);
    }
}

} // namespace yolo
