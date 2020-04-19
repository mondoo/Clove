#include "Clove/Log.hpp"

#if !_M_CEE

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace clv{
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;

	void Log::init(){
		const std::string_view consolePattern = "%^[%T] %n: %v%$";
		const std::string_view filePattern = "[%D %T][%l] %n: %v";

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern(consolePattern.data());
		consoleSink->set_level(spdlog::level::info);

		std::vector<spdlog::sink_ptr> coreSinks;
		coreSinks.emplace_back(consoleSink);
		coreSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Clove.log", true));
		coreSinks[1]->set_pattern(filePattern.data());
		coreSinks[1]->set_level(spdlog::level::trace);

		std::vector<spdlog::sink_ptr> clientSinks;
		clientSinks.emplace_back(consoleSink);
		clientSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Application.log", true));
		clientSinks[1]->set_pattern(filePattern.data());
		clientSinks[1]->set_level(spdlog::level::trace);

		coreLogger = std::make_shared<spdlog::logger>("CLOVE", coreSinks.begin(), coreSinks.end());
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = std::make_shared<spdlog::logger>("APP", clientSinks.begin(), clientSinks.end());
		clientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Log::getCoreLogger(){
		return coreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::getClientLogger(){
		return clientLogger;
	}
}

#endif