#include "ConfigManager.h"
#include <nlohmann/json.hpp>

namespace ai::utils {

ConfigManager::ConfigManager() : m_isLoaded(false) {
    // Defaults handled in struct definition
}

ConfigManager::~ConfigManager() {
    // Clean up if file held resources (if we implemented file locking)
}

bool ConfigManager::load(const std::string& filepath) {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    nlohmann::json j;
    file >> j;
    
    // Example parsing logic (Simplified for Step 1)
    if (j.contains("system")) {
        // Populate m_config members from j
    }

    m_isLoaded = true;
    return true;
}

void ConfigManager::reload() {
    // Re-open and parse
}

const ConfigTypes& ConfigManager::get() const {
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    return m_config;
}

void ConfigManager::setFPS(int fps) {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_config.fps = fps;
}

void ConfigManager::setConfidence(float conf) {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_config.confidence = conf;
}

bool ConfigManager::save(const std::string& filepath) const {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    // Serialize m_config to JSON
    // return file << j;
    return true; 
}

} // namespace ai::utils
