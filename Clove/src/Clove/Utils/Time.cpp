#include "clvpch.hpp"
#include "Time.hpp"

namespace clv::utl{
	float Time::deltaTime = 0.0f;

	float Time::getDeltaTime(){
		return deltaTime;
	}
}