#include "Tunic/Application.hpp"

#include "Tunic/Input.hpp"
#include "Tunic/LayerStack.hpp"
#include "Tunic/Layer.hpp"
#include "Clove/Core/Utils/DeltaTime.hpp"
#include "Tunic/ECS/Core/World.hpp"
#include "Tunic/Rendering/Renderer.hpp"
#include "Tunic/Rendering/Renderer2D.hpp"

#include <Clove/Platform/Core/Window.hpp>
#include <Clove/Platform/Core/Platform.hpp>

using namespace clv;

namespace tnc{
	Application* Application::instance = nullptr;

	Application::Application(gfx::API api){
		CLV_PROFILE_BEGIN_SESSION("Application cycle", "Profile-Cycle.json");

		Log::init();

		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		platformInstance = clv::plt::createPlatformInstance();

		mainWindow = platformInstance->createWindow({ "Tunic Application", 1280, 720, api });
		mainWindow->onWindowCloseDelegate.bind(&tnc::Application::stop, this);
		mainWindow->setVSync(true);

		renderer = std::make_unique<rnd::Renderer>(*mainWindow);
		renderer2D = std::make_unique<rnd::Renderer2D>(*mainWindow);

		ecsWorld = std::make_unique<ecs::World>();
		layerStack = std::make_unique<LayerStack>();

		CLV_LOG_INFO("Successfully initialised Clove");

		prevFrameTime = std::chrono::system_clock::now();
	}

	Application::Application(clv::gfx::API api, const clv::plt::Window& parentWindow, const clv::mth::vec2i& windowPosition, const clv::mth::vec2i& windowSize){
		CLV_PROFILE_BEGIN_SESSION("Application cycle", "Profile-Cycle.json");
		
		Log::init();

		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		platformInstance = clv::plt::createPlatformInstance();

		mainWindow = platformInstance->createChildWindow(parentWindow, windowPosition, windowSize, api);
		mainWindow->onWindowCloseDelegate.bind(&tnc::Application::stop, this);
		mainWindow->setVSync(true);

		renderer = std::make_unique<rnd::Renderer>(*mainWindow);
		renderer2D = std::make_unique<rnd::Renderer2D>(*mainWindow);

		ecsWorld = std::make_unique<ecs::World>();
		layerStack = std::make_unique<LayerStack>();

		CLV_LOG_INFO("Successfully initialised Clove");

		prevFrameTime = std::chrono::system_clock::now();
	}

	Application::~Application(){
		CLV_PROFILE_END_SESSION();
	}

	void Application::update(){
		CLV_PROFILE_FUNCTION();

		auto currFrameTime = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
		prevFrameTime = currFrameTime;

		mainWindow->beginFrame();

		//TODO:
		//Will need process the mouse and keyboard events here eventually

		for(const auto& layer : *layerStack){
			layer->onUpdate(deltaSeonds.count());
		}

		renderer->begin();
		renderer2D->begin();

		ecsWorld->update(deltaSeonds.count());

		renderer->end();
		renderer2D->end();

		{
			CLV_PROFILE_SCOPE("Window::endFrame");
			mainWindow->endFrame();
		}
	}

	void Application::stop(){
		currentState = ApplicationState::stopped;
	}

	ApplicationState Application::getState() const{
		return currentState;
	}

	void Application::pushLayer(std::shared_ptr<Layer> layer){
		layerStack->pushLayer(layer);
	}

	void Application::pushOverlay(std::shared_ptr<Layer> overlay){
		layerStack->pushOverlay(overlay);
	}

	Application& Application::get(){
		return *instance;
	}

	ecs::World& Application::getWorld(){
		return *ecsWorld;
	}

	plt::Window& Application::getMainWindow() const{
		return *mainWindow;
	}

	void Application::setMainWindow(const std::shared_ptr<plt::Window>& window){
		mainWindow = window;
	}

	clv::gfx::GraphicsFactory& Application::getGraphicsFactory(){
		return mainWindow->getGraphicsFactory();
	}

	rnd::Renderer* Application::getRenderer(){
		return renderer.get();
	}

	rnd::Renderer2D* Application::getRenderer2D(){
		return renderer2D.get();
	}
}