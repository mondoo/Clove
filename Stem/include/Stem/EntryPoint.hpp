#pragma once

#include "Stem/Application.hpp"

int main() {
	CLV_PROFILE_BEGIN_SESSION("Startup", "GarlicProfile-Startup.json");
	garlic::Application app{};
	CLV_PROFILE_END_SESSION();

	CLV_PROFILE_BEGIN_SESSION("Runtime", "GarlicProfile-Runtime.json");
	while(app.getState() != garlic::Application::State::Stopped){
        app.tick();
    }
	CLV_PROFILE_END_SESSION();

	return 0;
}