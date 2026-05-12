/**
 * @file logger.cpp
 * @brief Implementation of thread-safe logger for the traffic simulation
 * @author Traffic Simulation Team
 * @version 1.0
 */

#include "common/logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

namespace traffic_simulation {

Logger::Logger() : use_file_(false) {}

Logger::~Logger() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::ostringstream oss;
    oss << "[" << getCurrentTimestamp() << "] "
        << "[" << logLevelToString(level) << "] "
        << message;

    std::string formatted_message = oss.str();

    // Output to console
    if (level == LogLevel::ERROR) {
        std::cerr << formatted_message << std::endl;
    } else {
        std::cout << formatted_message << std::endl;
    }

    // Output to file if configured
    if (use_file_ && file_stream_.is_open()) {
        file_stream_ << formatted_message << std::endl;
        file_stream_.flush();
    }
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

void Logger::setOutputFile(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
    
    file_stream_.open(filepath, std::ios::app);
    use_file_ = file_stream_.is_open();
}

std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

} // namespace traffic_simulation
