#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace clv{
	class Log{
		//VARIABLES
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

		//FUNCTIONS
	public:
		CLV_API static void init();

		CLV_API inline static std::shared_ptr<spdlog::logger>& getCoreLogger();
		CLV_API inline static std::shared_ptr<spdlog::logger>& getClientLogger();
	};
}

#include "Clove/Log.inl"

#if CLV_ENGINE
	//Core log macros
	#define CLV_TRACE(...)		::clv::Log::getCoreLogger()->trace(__VA_ARGS__)
	#define CLV_INFO(...)		::clv::Log::getCoreLogger()->info(__VA_ARGS__)
	#define CLV_WARN(...)		::clv::Log::getCoreLogger()->warn(__VA_ARGS__)
	#define CLV_ERROR(...)		::clv::Log::getCoreLogger()->error(__VA_ARGS__)
	#define CLV_CRITICAL(...)	::clv::Log::getCoreLogger()->critical(__VA_ARGS__)
#else
	//Client log macros
	#define CLV_TRACE(...)		::clv::Log::getClientLogger()->trace(__VA_ARGS__)
	#define CLV_INFO(...)		::clv::Log::getClientLogger()->info(__VA_ARGS__)
	#define CLV_WARN(...)		::clv::Log::getClientLogger()->warn(__VA_ARGS__)
	#define CLV_ERROR(...)		::clv::Log::getClientLogger()->error(__VA_ARGS__)
	#define CLV_CRITICAL(...)	::clv::Log::getClientLogger()->critical(__VA_ARGS__)
#endif