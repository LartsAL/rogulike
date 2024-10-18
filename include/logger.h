#pragma once

#include <string>

enum class LogLevel {
    DBG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static auto getInstance() -> Logger&;

    auto log(LogLevel level, const std::string& message, bool consoleLogging = true) const -> void;

private:
    Logger();
    static auto getCurrentTime(const std::string& format) -> std::string;
    static auto getLogLevelText(LogLevel level) -> std::string;
    static auto getLogLevelColor(LogLevel level) -> std::string;

    std::string logFilesPath;
    std::string logFileName;
};

#ifdef DEBUG
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DBG, msg);
#else
#define LOG_DEBUG(msg)
#endif

#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg);
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg);
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg);
#define LOG_FATAL(msg) Logger::getInstance().log(LogLevel::FATAL, msg);
