#pragma once

#include "Stem/Application.hpp"

int main() {
	clv::Log::init();

	stm::Application app{};

	app.start();
	app.run();

	return 0;
}