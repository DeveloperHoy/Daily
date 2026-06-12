#include "logger.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace yolo {

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::init(const std::string& log_file_path) {
    log_file_ = std::ofstream(log_file_path);
    log_file_opened_ = log_file_.is_open();
}

void Logger::shutdown() {
    log_file_.close();
    log_file_opened_ = false;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    if (!should_log(level)) return;

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = format_timestamp(time);

    std::string log_str = "[" + timestamp + "] " + color_for_level(level) + " " + message;

    std::cout << log_str << std::endl;
    if (log_file_opened_) {
        log_file_ << log_str << std::endl;
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}

void Logger::set_level(LogLevel level) {
    log_level_ = level;
}

bool Logger::should_log(LogLevel level) const {
    return level >= log_level_;
}

std::string Logger::format_timestamp(std::time_t time) {
    auto tm = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::color_for_level(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "\033[36m[DEBUG]\033[0m ";
        case LogLevel::INFO:
            return "\033[32m[INFO]\033[0m ";
        case LogLevel::WARNING:
            return "\033[33m[WARNING]\033[0m ";
        case LogLevel::ERROR:
            return "\033[31m[ERROR]\033[0m ";
        case LogLevel::FATAL:
            return "\033[35m[FATAL]\033[0m ";
        default:
            return "[LOG] ";
    }
}

} // namespace yolo
