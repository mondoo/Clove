#include "spdlog/sinks/stdout_color_sinks.h"

namespace clv{
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;

	void Log::init(){
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("CLOVE");
		clientLogger = spdlog::stdout_color_mt("APP");

	#if CLV_DEBUG
		coreLogger->set_level(spdlog::level::trace);
	#elif CLV_DEVELOPMENT
		coreLogger->set_level(spdlog::level::debug);
	#elif CLV_PROFILING
		coreLogger->set_level(spdlog::level::info);
	#elif CLV_RELEASE
		coreLogger->set_level(spdlog::level::err);
	#endif

		//TODO: Should implement a way for client applications to control the log level
		clientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Log::getCoreLogger(){
		return coreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::getClientLogger(){
		return clientLogger;
	}
}
