#include "Clove/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

clv::Log::Context garlicLogContext = clv::Log::createContext("GARLIC", "GarlicLog.txt");

namespace clv {
	Log::Context Log::createContext(std::string_view loggerName, std::string_view fileName) {
		const std::string consolePattern = "%^[%T] %n: %v%$";
		const std::string filePattern = "[%D %T][%l] %n: %v";

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName.data(), true);

		consoleSink->set_pattern("%^[%T] %n: %v%$");
		fileSink->set_pattern("[%D %T][%l] %n: %v");

#if CLV_DEBUG
		consoleSink->set_level(spdlog::level::trace);
#else
		consoleSink->set_level(spdlog::level::info);
#endif
		fileSink->set_level(spdlog::level::trace);

		std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
		auto logger = std::make_shared<spdlog::logger>(loggerName.data(), sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);

		return Log::Context{ logger };
	}
}