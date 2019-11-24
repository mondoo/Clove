#include "Application.hpp"

#include "Core/Platform/Window.hpp"
#include "Core/Input/Input.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Layer.hpp"
#include "Core/Utils/DeltaTime.hpp"
#include "Core/ECS/Manager.hpp"

#include "Core/Graphics/RenderCommand.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsApplication.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Platform/Linux/LinuxApplication.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Platform/Mac/MacApplication.hpp"
#endif

namespace clv::plt{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		Log::init();

		CLV_LOG_INFO("Successfully initialised Clove");

		prevFrameTime = std::chrono::system_clock::now();
	}

	Application::~Application() = default;

	void Application::start(){
		//TODO: Added a 'start' function to handle not calling a virtual from the ctor
		//Would like as minimal api as possible when starting the application

		gfx::RenderCommand::initialise(getPlatformPreferedAPI());

		window = createWindow();
		window->onWindowCloseDelegate.bind(&Application::onWindowClose, this);
		window->setVSync(true);

		gfx::RenderCommand::setClearColour({ 1.0f, 0.54f, 0.1f, 1.0f });

		ecsManager = std::make_unique<ecs::Manager>();
		layerStack = std::make_unique<LayerStack>();
	}

	void Application::update(){
		auto currFrameTime = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
		prevFrameTime = currFrameTime;

		window->beginFrame();

		//TODO:
		//Will need process the mouse and keyboard events here eventually

		for(auto layer : *layerStack){
			layer->onUpdate(deltaSeonds.count());
		}

		ecsManager->update(deltaSeonds.count());

		window->endFrame();
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

	Window& Application::getWindow(){
		return *window;
	}

	ecs::Manager& Application::getManager(){
		return *ecsManager;
	}

	std::unique_ptr<Application> Application::createApplication(){
	#if CLV_PLATFORM_WINDOWS
		return std::make_unique<WindowsApplication>();
	#elif CLV_PLATFORM_LINUX
		return std::make_unique<LinuxApplication>();
	#elif CLV_PLATFORM_MACOS
		return std::make_unique<MacApplication>();
	#endif
	}

	void Application::onWindowClose(){
		stop();
	}
}