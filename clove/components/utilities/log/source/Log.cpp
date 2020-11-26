#include "Clove/Log/Log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace garlic::clove {
    class SpdLogOutput : public Logger::Output {
        //VARIABLES
    private:
        std::shared_ptr<spdlog::logger> logger;

        //FUNCTIONS
    public:
        SpdLogOutput() {
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("CLOVE_LOG.txt", true);

            consoleSink->set_pattern("%^[%T] %v%$");
            fileSink->set_pattern("[%D %T][%l] %v");

            consoleSink->set_level(spdlog::level::trace);
            fileSink->set_level(spdlog::level::trace);

            std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
            logger = std::make_shared<spdlog::logger>("GARLIC_LOGGER", sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::trace);
        };

        void doLog(LogLevel level, std::string_view msg) override {
            switch(level) {
                case LogLevel::Trace:
                    logger->trace(msg);
                    break;
                case LogLevel::Info:
                    logger->info(msg);
                    break;
                case LogLevel::Warning:
                    logger->warn(msg);
                    break;
                case LogLevel::Error:
                    logger->error(msg);
                    break;
            }
        }
    };

    std::unique_ptr<Logger::Output> Logger::out = std::make_unique<SpdLogOutput>();
}