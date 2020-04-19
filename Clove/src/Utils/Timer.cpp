#include "Clove/Utils/Timer.hpp"

#include <thread>

#if CLV_ENABLE_PROFILING
namespace clv::utl{
	Timer::Timer()
		: currentSession(nullptr), profileCount(0){
	}

	Timer::~Timer() = default;

	Timer& Timer::get(){
		static Timer instance;
		return instance;
	}

	void Timer::beginSession(const std::string& name, const std::string& filepath){
		outputStream.open(filepath);
		writeHeader();
		currentSession = new TimerSession{ name };
	}

	void Timer::endSession(){
		writeFooter();
		outputStream.close();

		delete currentSession;
		currentSession = nullptr;

		profileCount = 0;
	}

	void Timer::writeHeader(){
		outputStream << "{\"otherData\": {},\"traceEvents\":[";
		outputStream.flush();
	}

	void Timer::writeProfile(const ProfileResult& result){
		if(profileCount++ > 0){
			outputStream << ",";
		}

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		outputStream << "{";
		outputStream << "\"cat\":\"function\",";
		outputStream << "\"dur\":" << (result.end - result.start) << ',';
		outputStream << "\"name\":\"" << name << "\",";
		outputStream << "\"ph\":\"X\",";
		outputStream << "\"pid\":0,";
		outputStream << "\"tid\":" << result.threadID << ",";
		outputStream << "\"ts\":" << result.start;
		outputStream << "}";

		outputStream.flush();
	}

	void Timer::writeFooter(){
		outputStream << "]}";
		outputStream.flush();
	}

	ScopedTimerClock::ScopedTimerClock(const std::string& name)
		: name(name){
		startTime = std::chrono::high_resolution_clock::now();
	}

	ScopedTimerClock::~ScopedTimerClock(){
		auto endTime = std::chrono::high_resolution_clock::now();

		int64 start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
		int64 end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		uint32 threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		Timer::get().writeProfile({ name, start, end, threadID });
	}
}
#endif

