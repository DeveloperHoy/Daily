#include "Application/Application.h"
#include <iostream>

namespace ai::application {

Application::Application(int argc, char* argv[]) {
    // Initialize Logger
    m_logger.setLevel(ai::utils::LogLevel::INFO);
    
    // Check CLI args
    checkArgs(argc, argv);

    // Initialize ThreadPool (Assuming 4-8 threads for production)
    // In production, we calculate based on CPU cores (std::thread::hardware_concurrency())
    size_t threads = std::thread::hardware_concurrency();
    m_threadPool = std::make_shared<ai::utils::ThreadPool>(threads);

    // Initialize Config (Load if file provided by CLI, else default)
    if (!m_configManager.isLoaded()) {
        m_configManager.load("config/default.json");
    }

    initSystem();
}

void Application::initSystem() {
    // Sub-modules (Camera, Detector, Recorder) will be instantiated here
    // We create them with the thread pool or spawn dedicated threads per camera
    // For Step 1, we just log that we are ready.
    m_logger.log(ai::utils::LogLevel::INFO, "Core Application Initialized. Waiting for input.");
}

void Application::checkArgs(int argc, char* argv[]) {
    if (argc == 2) {
        m_configManager.load(argv[1]);
    }
}

void Application::run() {
    // Main Loop Logic
    while (m_state == AppState::RUNNING) {
        // Perform task, add to thread pool
    }
}

void Application::stop() {
    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        m_state = AppState::STOPPED;
    }
    m_stateCV.notify_all();
}

AppState Application::getState() const {
    std::shared_lock<std::mutex> lock(m_stateMutex); // Wait for lock
    return m_state;
}

const ai::utils::ConfigManager& Application::getConfigManager() const {
    return m_configManager;
}

void Application::setLogLevel(ai::utils::LogLevel level) {
    m_logger.setLevel(level);
}

void Application::startRecorder() {
    m_logger.log(ai::utils::LogLevel::INFO, "Starting Video Recorder module...");
    // Implementation would go here
}

void Application::startRenderer() {
    m_logger.log(ai::utils::LogLevel::INFO, "Starting Video Renderer module...");
    // Implementation would go here
}

} // namespace ai::application
