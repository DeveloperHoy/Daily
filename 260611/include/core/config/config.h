#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace yolo {

struct ModelConfig {
    std::string model_path = "";
    ModelType type = ModelType::YOLOv9S;
    bool enable_gpu = true;
    bool draw_boxes = true;
    bool draw_labels = true;
    bool save_results = false;
};

class ConfigManager {
public:
    static ConfigManager& instance();
    void load_config(const std::string& config_file);
    void save_config(const std::string& config_file) const;
    bool reload_config(const std::string& config_file);

    void set_model_path(const std::string& path);
    std::string get_model_path() const { return model_path_; }

    void set_conf_threshold(float threshold);
    float get_conf_threshold() const { return conf_threshold_; }

    void set_iou_threshold(float threshold);
    float get_iou_threshold() const { return iou_threshold_; }

    void set_device(int device);
    int get_device() const { return device_; }

    void set_gpu_enabled(bool enabled);
    bool get_gpu_enabled() const { return gpu_enabled_; }

    void set_draw_boxes(bool draw);
    bool get_draw_boxes() const { return draw_boxes_; }

    void set_save_results(bool save);
    bool get_save_results() const { return save_results_; }

    std::vector<std::string> get_model_types() const { return model_types_; }

private:
    ModelConfig config_;
    std::vector<std::string> model_types_;
    float conf_threshold_ = 0.5f;
    float iou_threshold_ = 0.5f;
    int device_ = 0;
    bool gpu_enabled_ = true;
    bool draw_boxes_ = true;
    bool save_results_ = false;

    std::vector<std::string> parse_line(const std::string& line);
    std::string trim(const std::string& str);

    ~ConfigManager() = default;
};

} // namespace yolo
