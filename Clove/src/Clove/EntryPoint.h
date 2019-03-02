#pragma once

#ifdef CLV_PLATFORM_WINDOWS

extern clv::Application* clv::createApplication();

int main(int argc, char** argv){
	clv::Log::init();
	CLV_CORE_WARN("Initialised Log!");
	int a = 5;
	CLV_INFO("Hello Var = {0}", a);

	auto* app = clv::createApplication();
	app->run();
	delete app;
}

#endif // CLV_PLATFORM_WINDOWS
