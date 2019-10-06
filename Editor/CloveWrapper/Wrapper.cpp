#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include "Clove/Application.hpp"

class Editor : public clv::Application{
public:
	Editor(){
		
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