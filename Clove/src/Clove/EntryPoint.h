#pragma once

#ifdef CLV_PLATFORM_WINDOWS

extern clv::Application* clv::createApplication();

int main(int argc, char** argv){
	auto* app = clv::createApplication();
	app->run();
	delete app;
}

#endif // CLV_PLATFORM_WINDOWS
