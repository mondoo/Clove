#pragma once

#include <string>
#include <string_view>
#include <memory>

#define CLOVE_DECLARE_LOG_CATEGORY(categoryName)                \
    struct LOG_CATEGORY_##categoryName {                         \
        static std::string_view constexpr name{ #categoryName }; \
    };

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

#define CLOVE_LOG(category, level, ...) ::garlic::clove::Logger::log(category::name, level, __VA_ARGS__);

#if CLOVE_ENABLE_ASSERTIONS
    #define CLOVE_ASSERT(x, ...)                                                                                        \
        {                                                                                                                \
            if(!(x)) {                                                                                                   \
                CLOVE_LOG(LOG_CATEGORY_CLOVE, ::garlic::clove::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                CLOVE_DEBUG_BREAK;                                                                                      \
            }                                                                                                            \
        }
#else
    #define CLOVE_ASSERT(x, ...) (x)
#endif

#include "Log.inl"

CLOVE_DECLARE_LOG_CATEGORY(CLOVE)