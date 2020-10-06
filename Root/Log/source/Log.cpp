#include "Root/Log/Log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if GARLIC_DEBUG
garlic::root::LogContext garlicLogContext = garlic::root::createLogContext(garlic::root::LogLevel::Trace, "GARLIC", "GarlicLog.txt");
#else
garlic::root::LogContext garlicLogContext = garlic::root::createLogContext(garlic::root::LogLevel::Info, "GARLIC", "GarlicLog.txt");
#endif

namespace garlic::inline root {
    static spdlog::level::level_enum getSpdlogLevel(LogLevel level) {
        switch(level) {
            case LogLevel::Trace:
                return spdlog::level::trace;
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Warning:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Critical:
                return spdlog::level::critical;
        }
    }

    LogContext createLogContext(LogLevel consoleLogLevel, std::string_view loggerName, std::string_view fileName) {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName.data(), true);

        consoleSink->set_pattern("%^[%T] %n: %v%$");
        fileSink->set_pattern("[%D %T][%l] %n: %v");

        consoleSink->set_level(getSpdlogLevel(consoleLogLevel));
        fileSink->set_level(spdlog::level::trace);

        std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
        auto logger = std::make_shared<spdlog::logger>(loggerName.data(), sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::trace);

        return LogContext{ logger };
    }
}