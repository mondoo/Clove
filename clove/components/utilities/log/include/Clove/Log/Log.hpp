#pragma once

#include <string>
#include <string_view>
#include <memory>

#define GARLIC_DECLARE_LOG_CATEGORY(categoryName)                \
    struct LOG_CATEGORY_##categoryName {                         \
        static std::string_view constexpr name{ #categoryName }; \
    };

namespace garlic::clove {
    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
}

namespace garlic::clove {
    class Logger {
        //TYPES
    public:
        class Output {
            //FUNCTIONS
        public:
            virtual ~Output() = default;

            virtual void doLog(LogLevel level, std::string_view msg) = 0;
        };

        //VARIABLES
    private:
        static std::unique_ptr<Output> out;

        //FUNCTIONS
    public:
        static inline void setOutput(std::unique_ptr<Output> newOut);

        template<typename... Args>
        static void log(std::string_view category, LogLevel level, std::string_view msg, Args &&... args);
    };
}

#define GARLIC_LOG(category, level, ...) ::garlic::clove::Logger::log(category::name, level, __VA_ARGS__);

#if GARLIC_ENABLE_ASSERTIONS
    #define GARLIC_ASSERT(x, ...)                                                                                        \
        {                                                                                                                \
            if(!(x)) {                                                                                                   \
                GARLIC_LOG(LOG_CATEGORY_GARLIC, ::garlic::clove::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                GARLIC_DEBUG_BREAK;                                                                                      \
            }                                                                                                            \
        }
#else
    #define GARLIC_ASSERT(x, ...) (x)
#endif

GARLIC_DECLARE_LOG_CATEGORY(GARLIC)

#include "Log.inl"