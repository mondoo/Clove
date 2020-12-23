#pragma once

#include <memory>
#include <string>
#include <string_view>

#define CLOVE_DECLARE_LOG_CATEGORY(categoryName)                 \
    struct LOG_CATEGORY_##categoryName {                         \
        static std::string_view constexpr name{ #categoryName }; \
    };

namespace spdlog {
    class logger;
}

namespace garlic::clove {
    enum class LogLevel {
        Trace,
        Info,
        Warning,
        Error,
    };
}

namespace garlic::clove {
    class Logger {
        //VARIABLES
    private:
        std::unique_ptr<spdlog::logger> logger;

        //FUNCTIONS
    public:
        Logger();
        ~Logger();

        static inline Logger &get();

        template<typename... Args>
        void log(std::string_view category, LogLevel level, std::string_view msg, Args &&... args);

    private:
        void doLog(LogLevel level, std::string_view msg);
    };
}

#define CLOVE_LOG(category, level, ...) ::garlic::clove::Logger::get().log(category::name, level, __VA_ARGS__);
#if CLOVE_DEBUG
    #define CLOVE_LOG_DEBUG(category, level, ...) ::garlic::clove::Logger::get().log(category::name, level, __VA_ARGS__);
#else
    #define CLOVE_LOG_DEBUG(category, level, ...)
#endif

#if CLOVE_ENABLE_ASSERTIONS
    #define CLOVE_ASSERT(x, ...)                                                                                       \
        {                                                                                                              \
            if(!(x)) {                                                                                                 \
                CLOVE_LOG(LOG_CATEGORY_CLOVE, ::garlic::clove::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                CLOVE_DEBUG_BREAK;                                                                                     \
            }                                                                                                          \
        }
#else
    #define CLOVE_ASSERT(x, ...) (x)
#endif

#include "Log.inl"

CLOVE_DECLARE_LOG_CATEGORY(CLOVE)