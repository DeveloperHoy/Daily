#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace yolo {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static Logger& instance();
    void init(const std::string& log_file_path);
    void shutdown();

    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);

    void set_level(LogLevel level);
    LogLevel get_level() const { return log_level_; }

    bool should_log(LogLevel level) const;
    bool log_file_opened() const { return log_file_opened_; }

private:
    LogLevel log_level_ = LogLevel::INFO;
    bool log_file_opened_ = false;
    std::ofstream log_file_;
    std::mutex log_mutex_;
    std::time_t last_log_time_;

    static std::string format_timestamp(std::time_t time);
    static std::string color_for_level(LogLevel level);

    ~Logger() = default;
};

} // namespace yolo
