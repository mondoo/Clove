#pragma once

extern clv::Application* clv::createApplication();

int main(int argc, char** argv){
	try{
		clv::Log::init();
		auto* app = clv::createApplication();
		app->run();
		delete app;
	} catch(const clv::CloveException& e){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	#endif
	} catch(const std::exception& e){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	#endif
	} catch(...){
	#if CLV_PLATFORM_WINDOWS
		MessageBoxA(nullptr, "No details available", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	#endif
	}

	return 0;
}
