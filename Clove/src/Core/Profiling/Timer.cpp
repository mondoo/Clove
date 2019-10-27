#include "Timer.hpp"

#if CLV_ENABLE_PROFILING
namespace clv{
	namespace profiling{
		Timer::Timer(const std::string& name)
			:name(name){
			start = std::chrono::high_resolution_clock::now();
		}

		Timer::~Timer(){
			end = std::chrono::high_resolution_clock::now();
			duration = end - start;

			CLV_LOG_INFO("Timer completed inside {0} after {1}s", name, duration.count() * 1000.0f);
		}
	}
}
#endif