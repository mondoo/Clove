#include <Bulb/Bulb.hpp>

#include <Bulb/Rendering/ForwardRenderer3D.hpp>

int main(){
	auto platform = clv::plt::createPlatformInstance();
	auto mainWindow = platform->createWindow({ "Vulkan refactor!", 1280, 720 });
	mainWindow->setVSync(true);

	auto renderer = blb::rnd::ForwardRenderer3D(*mainWindow, clv::gfx::API::Vulkan);

	while(mainWindow->isOpen()) {
		mainWindow->processInput();
		if(mainWindow->getKeyboard().isKeyPressed(clv::Key::Escape)) {
			break;
		}

		renderer.begin();

		//TODO: Submit mesh data

		renderer.end();
	}
}