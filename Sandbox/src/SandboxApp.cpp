#include <Clove.hpp>

#include "TestLayer.hpp"
#include "GameLayer.hpp"

int main(int argc, char** argv){
	try{
		auto app = clv::plt::Application::createApplication();

		app->pushLayer(std::make_shared<TestLayer>());
		//app->pushLayer(std::make_shared<GameLayer>());

		while(app->getState() == clv::plt::ApplicationState::running){
			app->update();
		}
	} catch(const clv::CloveException & e){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	#endif
	#if	CLV_RELEASE
	} catch(const std::exception & e){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	#endif
	} catch(...){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, "No details available", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	#endif
	#endif
	}
	
	return 0;
}
