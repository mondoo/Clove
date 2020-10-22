#pragma once

#include <spdlog/spdlog.h>
#include <string_view>

namespace garlic::inline root {
    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    struct LogContext {
    private:
        std::shared_ptr<spdlog::logger> logger;

    public:
        LogContext() = delete;
        LogContext(std::shared_ptr<spdlog::logger> logger)
            : logger(std::move(logger)) {
        }

        template<typename... Args>
        void log(LogLevel level, const char* msg, Args&&... args);

        template<typename T>
        void log(LogLevel level, const T& msg);
    };

    LogContext createLogContext(LogLevel consoleLogLevel, std::string_view loggerName, std::string_view fileName);
}

#include "Log.inl"

#define GARLIC_LOG(context, level, ...) context.log(level, __VA_ARGS__);

#if GARLIC_ENABLE_ASSERTIONS
    #define GARLIC_ASSERT(x, ...)                                                                                  \
        {                                                                                                          \
            if(!(x)) {                                                                                             \
                GARLIC_LOG(garlicLogContext, garlic::root::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                GARLIC_DEBUG_BREAK;                                                                                \
            }                                                                                                      \
        }
#else
    #define GARLIC_ASSERT(x, ...) (x)
#endif

extern garlic::root::LogContext garlicLogContext;