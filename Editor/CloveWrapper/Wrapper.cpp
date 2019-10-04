#include "Wrapper.hpp"

//Reg C++ (Works)
//#include "clvpch.hpp"//TODO: See note in example layer
//#include <Clove.hpp>

//Reg C++/CLR (No works :( )
#include <memory>
#include <chrono>
#include "Clove/Application.hpp"

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

//Reg C++/CLR (No works :( )
void CloveWrapper::Wrapper::OpenClove(){
	//clv::Log::init();
	auto* app = clv::createApplication();
	app->run();
	delete app;
}

//Reg C++ (Works)
//void OpenClove(){
//	clv::Log::init();
//	auto* app = clv::createApplication();
//	app->run();
//	delete app;
//}
