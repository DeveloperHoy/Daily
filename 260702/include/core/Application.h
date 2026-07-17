#pragma once
#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include "AppState.h"
#include "Pipeline.h"
#include "utils/Logger.h"
#include "utils/ConfigManager.h"
#include "utils/ThreadPool.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

namespace ai::core {

class Application {
public:
    using PipelinePtr = std::shared_ptr<Pipeline>;
    using ConfigPtr = std::shared_ptr<ConfigManager>;
    using LoggerPtr = std::shared_ptr<Logger>;

    Application(int argc = 0, char* argv[] = nullptr);
    ~Application();
    
    // Non-copyable
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void initialize(int argc = 0, char* argv[] = nullptr);
    void start();
    void stop();
    void pause();
    void resume();

    AppState getState() const;
    void setState(AppState state);

    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] bool isPaused() const;

    // Sub-system accessors
    [[nodiscard]] ConfigPtr getConfig() const;
    [[nodiscard]] PipelinePtr getPipeline() const;
    [[nodiscard]] LoggerPtr getLogger() const;

    // Thread control
    void setMaxThreads(size_t count);
    size_t getMaxThreads() const;

    // Lifecycle hooks
    void preStart();
    void postStart();
    void preStop();

    // Command control
    void command(const std::string& cmd);

    [[nodiscard]] const std::vector<std::string>& getArgs() const;
    void parseArgs(int argc, char* argv[]);

private:
    bool validateConfig();
    bool validatePath(const std::string& path) const;

    AppState m_state;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_paused{false};

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<ConfigManager> m_config;
    std::shared_ptr<Pipeline> m_pipeline;
    std::shared_ptr<utils::ThreadPool> m_threadPool;

    size_t m_targetThreads = std::thread::hardware_concurrency();
    
    std::vector<std::string> m_args;
    std::mutex m_mutex;
    std::condition_variable m_stopCV;

    std::vector<std::unique_ptr<std::thread>> m_workerThreads;
};

} // namespace ai::core
