#include "Clove/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#if GARLIC_DEBUG
clv::Log::Context garlicLogContext = clv::Log::createContext(clv::Log::Level::Trace, "GARLIC", "GarlicLog.txt");
#else
clv::Log::Context garlicLogContext = clv::Log::createContext(clv::Log::Level::Info, "GARLIC", "GarlicLog.txt");
#endif

namespace clv {
	static spdlog::level::level_enum getSpdlogLevel(Log::Level level) {
		switch(level) {
			case Log::Level::Trace:
				return spdlog::level::trace;
			case Log::Level::Debug:
				return spdlog::level::debug;
			case Log::Level::Info:
				return spdlog::level::info;
			case Log::Level::Warning:
				return spdlog::level::warn;
			case Log::Level::Error:
				return spdlog::level::err;
			case Log::Level::Critical:
				return spdlog::level::critical;
		}
	}

	Log::Context Log::createContext(Log::Level consoleLogLevel, std::string_view loggerName, std::string_view fileName) {
		const std::string consolePattern = "%^[%T] %n: %v%$";
		const std::string filePattern = "[%D %T][%l] %n: %v";

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName.data(), true);

		consoleSink->set_pattern("%^[%T] %n: %v%$");
		fileSink->set_pattern("[%D %T][%l] %n: %v");

		consoleSink->set_level(getSpdlogLevel(consoleLogLevel));
		fileSink->set_level(spdlog::level::trace);

		std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
		auto logger = std::make_shared<spdlog::logger>(loggerName.data(), sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);

		return Log::Context{ logger };
	}
}