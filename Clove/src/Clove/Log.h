#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace clv{
	class CLV_API Log{
		//VARIABLES
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

		//FUNCTIONS
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger(){ return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger(){ return clientLogger; }
	};
}

//Core log macros
#define CLV_CORE_TRACE(...) ::clv::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CLV_CORE_INFO(...)	::clv::Log::getCoreLogger()->info(__VA_ARGS__)
#define CLV_CORE_WARN(...)	::clv::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CLV_CORE_ERROR(...) ::clv::Log::getCoreLogger()->error(__VA_ARGS__)
#define CLV_CORE_FATAL(...) ::clv::Log::getCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define CLV_TRACE(...)	::clv::Log::getClientLogger()->trace(__VA_ARGS__)
#define CLV_INFO(...)	::clv::Log::getClientLogger()->info(__VA_ARGS__)
#define CLV_WARN(...)	::clv::Log::getClientLogger()->warn(__VA_ARGS__)
#define CLV_ERROR(...)	::clv::Log::getClientLogger()->error(__VA_ARGS__)
#define CLV_FATAL(...)	::clv::Log::getClientLogger()->fatal(__VA_ARGS__)