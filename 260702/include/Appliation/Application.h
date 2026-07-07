#pragma once

#include <vector>
#include <memory>
#include "utils/Logger.h"
#include "utils/ThreadPool.h"
#include "utils/ConfigManager.h"

namespace ai::application {

enum class AppState {
    IDLE,
    RUNNING,
    PAUSED,
    STOPPED
};

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application();

    void run();
    void stop();

    // Get current state
    AppState getState() const;

    // Get references to managers
    const ai::utils::ConfigManager& getConfigManager() const;

    // Setters for system initialization
    void setLogLevel(ai::utils::LogLevel level);

    // Methods to trigger sub-systems
    void startRecorder();
    void startRenderer();

private:
    void initSystem();
    void checkArgs();

    ai::utils::Logger m_logger;
    ai::utils::ConfigManager m_configManager;
    std::shared_ptr<ai::utils::ThreadPool> m_threadPool;

    // App State
    AppState m_state = AppState::IDLE;

    // Lifecycle variables
    int m_targetFps = 30;
    std::vector<std::unique_ptr<std::thread>> m_workerThreads; // Future camera threads
    int m_currentFps = 0;

    // Thread safe flags
    std::mutex m_stateMutex;
    std::condition_variable m_stateCV;
};

} // namespace ai::application
