#pragma once

//As spdlog uses mutex we can't use it when building /clr
#ifndef _M_CEE
    #include <spdlog/spdlog.h>
    #include <string_view>

namespace garlic::clove {
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
        void log(LogLevel level, char const *msg, Args &&... args);

        template<typename T>
        void log(LogLevel level, T const &msg);
    };

    LogContext createLogContext(LogLevel consoleLogLevel, std::string_view loggerName, std::string_view fileName);
}

    #include "Log.inl"

    #define GARLIC_LOG(context, level, ...) context.log(level, __VA_ARGS__);
#else
namespace garlic::clove {
    struct LogContext {
    };
}

    #define GARLIC_LOG(context, level, ...)
#endif

#if GARLIC_ENABLE_ASSERTIONS
    #define GARLIC_ASSERT(x, ...)                                                                                   \
        {                                                                                                           \
            if(!(x)) {                                                                                              \
                GARLIC_LOG(garlicLogContext, garlic::clove::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                GARLIC_DEBUG_BREAK;                                                                                 \
            }                                                                                                       \
        }
#else
    #define GARLIC_ASSERT(x, ...) (x)
#endif

extern garlic::clove::LogContext garlicLogContext;