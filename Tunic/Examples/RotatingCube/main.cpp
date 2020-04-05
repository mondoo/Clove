#include <Tunic/Tunic.hpp>

#include "ExampleLayer.hpp"

int main() {
	//Start by initialising the Tunic Application. This wraps a lot of Clove set up
	tnc::Application app = tnc::Application(clv::plt::getPlatformPreferedAPI());

	//Push any layers onto the application that we want to start with
	app.pushLayer(std::make_shared<ExampleLayer>());

	//Then run the main loop for our application
	while(app.getState() == tnc::ApplicationState::running) {
		app.update();
	}

	return 0;
}