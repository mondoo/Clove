#pragma once

#include <chrono>
#include <fstream>

#if CLV_ENABLE_PROFILING
namespace clv::utl{
	struct ProfileResult{
		std::string name;

		int64 start = 0;
		int64 end = 0;

		uint32 threadID;
	};

	struct TimerSession{
		std::string name;
	};
}

namespace clv::utl{
	class Timer{
		//VARIABLES
	private:
		TimerSession* currentSession = nullptr;
		std::ofstream outputStream;
		uint32 profileCount = 0;

		//FUNCTIONS
	public:
		Timer();

		~Timer();

		static Timer& get();
	
		void beginSession(const std::string& name, const std::string& filepath = "results.json");
		void endSession();

		void writeHeader();
		void writeProfile(const ProfileResult& result);
		void writeFooter();
	};
}

namespace clv::utl{
	class ScopedTimerClock{
		//VARIABLES
	private:
		std::string name;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

		//FUNCTIONS
	public:
		ScopedTimerClock(const std::string& name);

		~ScopedTimerClock();
	};
}
#endif

#if CLV_ENABLE_PROFILING
	#define CLV_PROFILE_BEGIN_SESSION(name, filepath) ::clv::utl::Timer::get().beginSession(name, filepath)
	#define CLV_PROFILE_END_SESSION() ::clv::utl::Timer::get().endSession()
	#define CLV_PROFILE_SCOPE(name) ::clv::utl::ScopedTimerClock timer##__LINE__(name)
	#define CLV_PROFILE_FUNCTION() CLV_PROFILE_SCOPE(CLV_FUNCTION_NAME_PRETTY)
#else
	#define CLV_PROFILE_BEGIN_SESSION(name, filepath)
	#define CLV_PROFILE_END_SESSION()
	#define CLV_PROFILE_SCOPE(name)
	#define CLV_PROFILE_FUNCTION()
#endif
