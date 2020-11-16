#pragma once

#include "Stem/Application.hpp"

int main() {
	CLV_PROFILE_BEGIN_SESSION("Startup", "GarlicProfile-Startup.json");
	garlic::Application app{};

	app.start();
	CLV_PROFILE_END_SESSION();

	CLV_PROFILE_BEGIN_SESSION("Runtime", "GarlicProfile-Runtime.json");
	app.run();
	CLV_PROFILE_END_SESSION();

	return 0;
}