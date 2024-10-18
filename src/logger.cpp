#include "logger.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>

auto Logger::getInstance() -> Logger & {
    static Logger instance;
    return instance;
}

auto Logger::log(LogLevel level, const std::string& message, bool consoleLogging) const -> void {
    const std::string logMessage = std::format("[{}] [{}]: {}\n", getLogLevelText(level), getCurrentTime("%Y-%m-%d %H:%M:%S"), message);

    // File
    std::ofstream ofs(logFilesPath + "/" + logFileName, std::fstream::app);
    ofs << logMessage;
    ofs.close();

    // Console
    if (consoleLogging) {
        std::cout << getLogLevelColor(level) << logMessage << "\x1b[0m";
    }
}

Logger::Logger() {
    logFilesPath = "./logs";
    std::filesystem::create_directories(logFilesPath);
    logFileName = "logfile_" + getCurrentTime("%Y-%m-%d") + ".log";
    std::filesystem::remove(logFilesPath + "/" + logFileName);
}

auto Logger::getCurrentTime(const std::string& format) -> std::string {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    const std::tm* localTime = std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(localTime, format.c_str());
    return oss.str();
}

auto Logger::getLogLevelText(LogLevel level) -> std::string {
    switch (level) {
        case LogLevel::DBG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

auto Logger::getLogLevelColor(LogLevel level) -> std::string {
    switch (level) {
        case LogLevel::DBG: return "\x1b[38;5;243m";
        case LogLevel::INFO: return "\x1b[38;5;255m";
        case LogLevel::WARNING: return "\x1b[38;5;214m";
        case LogLevel::ERROR: return "\x1b[38;5;160m";
        case LogLevel::FATAL: return "\x1b[38;5;200m";
        default: return "";
    }
}
