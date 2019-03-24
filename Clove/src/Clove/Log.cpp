#include "clvpch.h"
#include "Log.h"

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
		clientLogger->set_level(spdlog::level::trace);
#elif CLV_RELEASE
		coreLogger->set_level(spdlog::level::err);
		clientLogger->set_level(spdlog::level::err);
#elif CLV_DIST
		coreLogger->set_level(spdlog::level::off);
		clientLogger->set_level(spdlog::level::off);
#endif
	}
}
