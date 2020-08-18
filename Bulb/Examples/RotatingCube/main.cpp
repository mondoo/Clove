#include "ExampleLayer.hpp"

#include <Bulb/Bulb.hpp>

int main() {
	//Create the platform instance we'll use to make OS calls
	std::unique_ptr<clv::plt::Platform> platformInstance = clv::plt::createPlatformInstance();

	//Create the window which we will render to
	std::shared_ptr<clv::plt::Window> window = platformInstance->createWindow({ "Rotating Cube", 1280, 720, clv::plt::getPlatformPreferedAPI() });
	window->setVSync(true);

	//Push on our example layer to the stack
	auto layerStack = std::make_unique<blb::LayerStack>();
	layerStack->pushLayer(std::make_shared<ExampleLayer>(window));

	//Cache of the 'previous frame time' that'll be used to calculate the delta time
	auto prevFrameTime = std::chrono::system_clock::now();

	//Run our program while the window is open
	while(window->isOpen()) {
		//Calculate the delta time
		auto currFrameTime = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
		prevFrameTime = currFrameTime;

		//Tell our window to process any input events
		window->processInput();

		//Update all of our layers
		for(auto&& layer : *layerStack) {
			layer->onUpdate(deltaSeconds.count());
		}
	}

	return 0;
}