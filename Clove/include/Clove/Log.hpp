#pragma once

#if !_M_CEE //Log is excluded from CLR builds until a better work around is found

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace clv{
	class Log{
		//VARIABLES
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

		//FUNCTIONS
	public:
		static std::shared_ptr<spdlog::logger>& getCoreLogger();
		static std::shared_ptr<spdlog::logger>& getClientLogger();

		static void init();
	};
}

#if CLV_ENGINE
	//Core log macros
	#define CLV_LOG_TRACE(...)		::clv::Log::getCoreLogger()->trace(__VA_ARGS__)
	#define CLV_LOG_DEBUG(...)		::clv::Log::getCoreLogger()->debug(__VA_ARGS__)
	#define CLV_LOG_INFO(...)		::clv::Log::getCoreLogger()->info(__VA_ARGS__)
	#define CLV_LOG_WARN(...)		::clv::Log::getCoreLogger()->warn(__VA_ARGS__)
	#define CLV_LOG_ERROR(...)		::clv::Log::getCoreLogger()->error(__VA_ARGS__)
	#define CLV_LOG_CRITICAL(...)	::clv::Log::getCoreLogger()->critical(__VA_ARGS__)
#else
	//Client log macros
	#define CLV_LOG_TRACE(...)		::clv::Log::getClientLogger()->trace(__VA_ARGS__)
	#define CLV_LOG_DEBUG(...)		::clv::Log::getClientLogger()->debug(__VA_ARGS__)
	#define CLV_LOG_INFO(...)		::clv::Log::getClientLogger()->info(__VA_ARGS__)
	#define CLV_LOG_WARN(...)		::clv::Log::getClientLogger()->warn(__VA_ARGS__)
	#define CLV_LOG_ERROR(...)		::clv::Log::getClientLogger()->error(__VA_ARGS__)
	#define CLV_LOG_CRITICAL(...)	::clv::Log::getClientLogger()->critical(__VA_ARGS__)
#endif
	
#else
	#define CLV_LOG_TRACE(...)	
	#define CLV_LOG_DEBUG(...)	
	#define CLV_LOG_INFO(...)	
	#define CLV_LOG_WARN(...)	
	#define CLV_LOG_ERROR(...)	
	#define CLV_LOG_CRITICAL(...)
#endif