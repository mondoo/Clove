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
			Timer(const std::string& name)
				:name(name){
				start = std::chrono::high_resolution_clock::now();
			}

			~Timer(){
				end = std::chrono::high_resolution_clock::now();
				duration = end - start;

				CLV_INFO("Timer completed inside {0} after {1}s", name, duration.count() * 1000.0f);
			}
		};
	}
}
#endif

#if CLV_ENABLE_PROFILING
	#define CLV_TIME_SCOPE(name) clv::profiling::Timer timer(name)
#else
	#define CLV_TIME_SCOPE(name)
#endif
