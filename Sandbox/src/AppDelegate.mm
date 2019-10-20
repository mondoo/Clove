#import "Platform/Mac/CloveMac.h"
#import "AppDelegate.hpp"

/*#import <Clove.hpp>*/

#import "TestLayer.hpp"
#import "GameLayer.hpp"

#import "Clove/Application.hpp"

@implementation AppDelegate

- (void)dealloc{
    [super dealloc];
}
	
- (void)applicationDidBecomeActive:(NSNotification *)aNotification{
	/*
	try{
	 */
		clv::Application* app = new clv::Application();

		//app->pushLayer(std::make_shared<TestLayer>());
		//app->pushLayer(std::make_shared<GameLayer>());

		//while(app->getState() == clv::ApplicationState::running){
		//	app->update();
		//}

		//delete app;
	/*
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
	 */
}

@end
