#include <Tunic/Tunic.hpp>

#include "ExampleLayer.hpp"

int main() {
	//Start by initialising the Tunic Application.
	//This wraps a lot of Clove functionality and holds the ECS
	tnc::Application app = tnc::Application(clv::plt::getPlatformPreferedAPI());

	//Add any systems to the ECS that we'll need for our application
	//The render system needs a renderer to submit to. Or we can give it a window to create
	//a renderer from
	app.getWorld().addSystem<tnc::ecs::_3D::RenderSystem>(app.getMainWindow());

	//Push any layers onto the application that we want to start with
	app.pushLayer(std::make_shared<ExampleLayer>());

	//Then run the main loop for our application
	while(app.getState() == tnc::ApplicationState::running) {
		app.update();
	}

	return 0;
}