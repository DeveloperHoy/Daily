#pragma once

#include <fstream>
#include <mutex>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

namespace ai::utils {

enum class LogLevel {
    INFO,
    WARN,
    ERROR,
    DEBUG
};

class Logger {
public:
    Logger();
    ~Logger();
    // Non-copyable, non-movable
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void setLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);
    void logTimestamp();
    
    void initializeLogFile(const std::string& path);
    bool isFilePathValid(const std::string& path) const;

private:
    void writeLog(LogLevel level, const std::string& message);
    std::ofstream& getOutputStream();

    LogLevel m_level;
    std::string m_logFilePath;
    std::ofstream m_fileStream;
    mutable std::mutex m_mutex;
};

} // namespace ai::utils
