/**
 * @file logger.hpp
 * @brief Thread-safe logger utility for the traffic simulation
 * @author Traffic Simulation Team
 * @version 1.0
 */

#pragma once

#include <string>
#include <mutex>
#include <fstream>

namespace traffic_simulation {

/**
 * @brief Log severity levels
 */
enum class LogLevel : uint8_t {
    INFO    = 0,
    WARNING = 1,
    ERROR   = 2
};

/**
 * @brief Thread-safe logger with timestamp support
 * 
 * Provides centralized logging functionality with automatic
 * timestamps and thread safety via mutex synchronization.
 */
class Logger {
public:
    /**
     * @brief Gets the singleton instance of the Logger
     * @return Reference to the Logger instance
     */
    static Logger& getInstance();

    /**
     * @brief Logs a message with the specified log level
     * @param level The severity level of the message
     * @param message The message to log
     */
    void log(LogLevel level, const std::string& message);

    /**
     * @brief Logs an info message
     * @param message The message to log
     */
    void info(const std::string& message);

    /**
     * @brief Logs a warning message
     * @param message The message to log
     */
    void warning(const std::string& message);

    /**
     * @brief Logs an error message
     * @param message The message to log
     */
    void error(const std::string& message);

    /**
     * @brief Sets the output file path
     * @param filepath Path to the log file
     */
    void setOutputFile(const std::string& filepath);

    // Non-copyable
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Non-movable
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    Logger();
    ~Logger();

    std::string getCurrentTimestamp() const;
    std::string logLevelToString(LogLevel level) const;

    std::mutex mutex_;
    std::ofstream file_stream_;
    bool use_file_;
};

} // namespace traffic_simulation
