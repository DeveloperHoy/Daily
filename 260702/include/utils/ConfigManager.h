#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <shared_mutex> // C++17
#include "Config/Config.h" // Forward declare or assume local header structure

namespace ai::utils {

// Simple mock structure for types, in production this would be a config schema
struct ConfigTypes {
    bool enabled = true;
    int fps = 30;
    float confidence = 0.5f;
    std::string inputSource = "";
    std::string outputRecording = "";
    std::string modelPath = "";
    std::unordered_map<std::string, std::string> regions; // ROI keys
    std::unordered_map<std::string, std::string> events;  // Event rules
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    bool load(const std::string& filepath);
    void reload();
    
    bool isLoaded() const;
    
    // Getters with thread safety
    // Returns reference to a thread-safe wrapper or shared_ptr
    const ConfigTypes& get() const;

    void setFPS(int fps);
    void setConfidence(float conf);

    bool save(const std::string& filepath) const;

private:
    ConfigTypes m_config;
    mutable std::shared_mutex m_mutex;
    bool m_isLoaded;
};

} // namespace ai::utils
