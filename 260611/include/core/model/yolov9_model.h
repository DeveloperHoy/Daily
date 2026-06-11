#pragma once
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <functional>
#include <map>
#include <atomic>
#include "../output/detection_result.h"
#include "../utils/logger.h"

namespace yolo {

class YOLOv9Model {
public:
    enum class ModelType { YOLOv9S, YOLOv9M, YOLOv9L, YOLOv9T };

    struct ModelConfig {
        ModelType type = ModelType::YOLOv9S;
        std::string input_layer_name = "input";
        std::vector<int> output_shapes = {640, 640, 25200}; // Shape: [H, W, num_boxes]
        std::vector<std::string> output_names = {"output1", "output2", "output3"};
        std::vector<int> class_ids = {0, 1, 2, 3}; // COCO classes
        std::vector<float> class_weights = {};
        std::vector<int> num_classes = {80};
        std::vector<int> num_anchors = {81, 152, 210};
        std::vector<std::vector<std::pair<int, int>>> anchor_boxes;
        std::vector<std::string> obj_classes = {};
    };

    class ModelHandle {
    public:
        ModelHandle(const std::string& model_path, const ModelConfig& config, 
                   bool use_cpu = false, Ort::LoggingLevel log_level = Ort::LoggingLevel::VERBOSE);
        ~ModelHandle();
        
        std::string path() const { return path_; }
        const ModelConfig& config() const { return config_; }
        
        std::vector<Ort::Value> infer(const std::vector<float>& input);
        void infer_batch(const std::vector<std::vector<float>>& inputs);

    private:
        void validate_model();
        std::vector<std::pair<int, int>> load_anchoes();
        std::vector<int> get_anchoes_shape();
        void parse_output_shape(std::vector<int>& shape);
        void parse_model_version();

    private:
        std::string path_;
        ModelConfig config_;
        std::unique_ptr<Ort::SessionHandle> session_;
        bool use_cpu_ = false;
        Ort::Environment env_;
        std::mutex mutex_;
        std::atomic<bool> valid_{false};
        std::vector<Ort::Value> cached_inputs_;
        std::vector<Ort::Value> cached_outputs_;
        std::chrono::steady_clock::time_point load_start_;
        std::chrono::steady_clock::time_point load_end_;
    };

public:
    YOLOv9Model(const std::string& model_path, const ModelConfig& config = ModelConfig());
    ~YOLOv9Model() = default;

    const ModelConfig& get_config() const { return config_; }
    ModelHandle create_session() const;
    
    std::vector<float> preprocess_input(const DetectionResult& result);
    std::vector<DetectionResult> postprocess(std::vector<float>& output, float conf_threshold);

    static bool validate_model(const std::string& path, const std::string& expected_ext = ".onnx");

private:
    void load_model();
    void validate_model();

    std::unique_ptr<Ort::Session> session_;
    std::unique_ptr<Ort::SessionOptions> session_options_;
    ModelConfig config_;
    std::vector<std::string> output_names_;
    std::vector<int> output_channel_ = {3}; // 80 classes + 4 boxes
    std::string default_class_name_ = "coco";
    Ort::SessionHandle session_handle_;
    bool initialized_ = false;
    std::chrono::steady_clock::time_point init_start_;
};

} // namespace yolo
