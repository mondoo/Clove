#import "Platform/Mac/CloveMac.h"
#import <Clove.hpp>

#import "TestLayer.hpp"
#import "GameLayer.hpp"

#import "AppDelegate.h"

int main(int argc, char** argv){
	int status = 0;
	@autoreleasepool {
		AppDelegate* delegate = [[[AppDelegate alloc] init] autorelease];
		[[NSApplication sharedApplication] setDelegate:delegate];
		status = NSApplicationMain(argc, (const char**)argv);
	}
	return status;
	
	try{
		//auto* app = new clv::Application();

		//app->pushLayer(std::make_shared<TestLayer>());
		//app->pushLayer(std::make_shared<GameLayer>());

		//while(app->getState() == clv::ApplicationState::running){
		//	app->update();
		//}

		//delete app;
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
