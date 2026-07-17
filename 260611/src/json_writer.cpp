#include "json_writer.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <nlohmann/json.hpp>

namespace yolo {

bool JsonWriter::write_json(const std::string& filepath, const DetectionResult& result) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json j;
    j["timestamp"] = result.timestamp_;
    j["class_name"] = result.class_name_;
    j["confidence"] = result.confidence_;
    j["box_x"] = result.box_x_y_[0];
    j["box_y"] = result.box_x_y_[1];
    j["box_w"] = result.box_x_y_[2];
    j["box_h"] = result.box_x_y_[3];

    file << j.dump(4);
    file.close();

    return true;
}

bool JsonWriter::write_batch_json(const std::string& filepath, std::vector<DetectionResult>& results) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json j;
    j["results"] = nlohmann::json::array();

    for (const auto& result : results) {
        nlohmann::json result_j;
        result_j["timestamp"] = result.timestamp_;
        result_j["class_name"] = result.class_name_;
        result_j["confidence"] = result.confidence_;
        result_j["box_x"] = result.box_x_y_[0];
        result_j["box_y"] = result.box_x_y_[1];
        result_j["box_w"] = result.box_x_y_[2];
        result_j["box_h"] = result.box_x_y_[3];
        j["results"].push_back(result_j);
    }

    file << j.dump(4);
    file.close();

    return true;
}

} // namespace yolo
