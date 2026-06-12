#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

#include "../utils/logger.h"

namespace yolo {

class DetectionResult {
public:
    std::vector<float> center_x_y;
    std::vector<float> box_x_y;
    std::string class_name;
    float class_id;
    float confidence;
    float objectness;
    float mask_center_x_y_w_h;

    DetectionResult() = default;

    static std::vector<DetectionResult> from_float_array(
        std::vector<std::vector<std::vector<float>>>& boxes,
        float* class_names,
        const std::vector<std::string>& anchors
    );

    void set_bbox(const std::string& anchor_type) { /* set default bbox based on anchor */ }

    bool is_valid() const { return confidence > 0.0f; }
};

class DetectionResults {
public:
    std::vector<DetectionResult> results;
    std::vector<std::string> class_names;
    
    DetectionResults() = default;

    void add(const DetectionResult& result);
    void clear();

    int get_total_count() const { return results.size(); }
};

} // namespace yolo
