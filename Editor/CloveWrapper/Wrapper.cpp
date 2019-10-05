#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include "Clove/Application.hpp"

/*
something inside https://help.veracode.com/reader/4EKhlLSMHm5jC8P8j3XccQ/UWyzuOKNbFbwQACRLkZVjA
helped debug - TODO: Set these up in CMake (bar RTC1)

also fix the C# platform
*/

class Editor : public clv::Application{
public:
	Editor(){
		//pushLayer(std::make_shared<TestLayer>());
		//pushLayer(std::make_shared<GameLayer>());
	}
	~Editor(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new Editor;
}

void CloveWrapper::Wrapper::OpenClove(){
	//clv::Log::init();
	auto* app = clv::createApplication();
	app->run();
	delete app;
}