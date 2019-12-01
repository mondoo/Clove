#pragma once

#include <chrono>

#if CLV_ENABLE_PROFILING
namespace clv{
	namespace profiling{
		struct Timer{
			//VARIABLES
		private:
			std::chrono::time_point<std::chrono::steady_clock> start;
			std::chrono::time_point<std::chrono::steady_clock> end;

			std::chrono::duration<float> duration;

			std::string name;

			//FUNCTIONS
		public:
			Timer(const std::string& name);

			~Timer();
		};
	}
}
#endif

#if CLV_ENABLE_PROFILING
	#define CLV_TIME_SCOPE(name) clv::profiling::Timer timer(name)
#else
	#define CLV_TIME_SCOPE(name)
#endif
