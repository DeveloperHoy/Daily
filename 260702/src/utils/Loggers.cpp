#include "Logger.h"
#include <ctime>

namespace ai::utils {

Logger::Logger() : m_level(LogLevel::INFO) {
    // Initialize to current year log
    // m_logFilePath = ""; 
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_level = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (static_cast<int>(level) < static_cast<int>(m_level)) {
        return;
    }

    writeLog(level, message);
}

void Logger::logTimestamp() {
    // Helper to prepend timestamp to current log stream
    // Implementation would format current time to string
}

void Logger::initializeLogFile(const std::string& path) {
    m_logFilePath = path;
    m_fileStream.open(path);
}

void Logger::writeLog(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    std::localtime_r(&time_t_now, &tm_now);

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");

    switch (level) {
        case LogLevel::INFO:
            oss << "[INFO] ";
            break;
        case LogLevel::WARN:
            oss << "[WARN] ";
            break;
        case LogLevel::ERROR:
            oss << "[ERROR] ";
            break;
        default:
            oss << "[LOG] ";
    }

    if (m_fileStream.is_open()) {
        oss << oss.str() << " " << message << std::endl;
        m_fileStream << oss.str() << std::endl;
    } else {
        // Fallback to stdout for console usage
        std::cout << oss.str() << " " << message << std::endl;
    }
}

std::ofstream& Logger::getOutputStream() {
    // Access logic handled internally
    return m_fileStream;
}

bool Logger::isFilePathValid(const std::string& path) const {
    return !path.empty();
}

} // namespace ai::utils
