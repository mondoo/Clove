#include "Clove/Core/Platform/Application.hpp"

#include "Clove/Core/Platform/Window.hpp"
#include "Clove/Core/Input/Input.hpp"
#include "Clove/Core/LayerStack.hpp"
#include "Clove/Core/Layer.hpp"
#include "Clove/Core/Utils/DeltaTime.hpp"
#include "Clove/Core/ECS/Manager.hpp"

#include "Clove/Core/Graphics/GraphicsGlobal.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Platform/Windows/WindowsApplication.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Platform/Linux/LinuxApplication.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Clove/Platform/Mac/MacApplication.hpp"
#endif

namespace clv::plt{
	Application* Application::instance = nullptr;

	Application::Application(gfx::API api){
		CLV_PROFILE_BEGIN_SESSION("Application cycle", "Profile-Cycle.json");

		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		Log::init();

		gfx::global::initialise(api);
		gfx::global::graphicsDevice->setClearColour({ 1.0f, 0.54f, 0.1f, 1.0f });

		ecsManager = std::make_unique<ecs::Manager>();
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

		ecsManager->update(deltaSeonds.count());

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

	ecs::Manager& Application::getManager(){
		return *ecsManager;
	}

	Window& Application::getMainWindow() const{
		return *mainWindow;
	}

	void Application::setMainWindow(const std::shared_ptr<Window>& window){
		mainWindow = window;
	}

	std::shared_ptr<Window> Application::openWindow(WindowType windowType, const WindowProps& props){
		auto window = createWindow(props);

		if(windowType == WindowType::MainWindow){
			setMainWindow(window);
		}

		return window;
	}

	std::shared_ptr<Window> Application::openChildWindow(WindowType windowType, const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		auto window = createChildWindow(parentWindow, position, size);

		if(windowType == WindowType::MainWindow){
			setMainWindow(window);
		}

		return window;
	}

	std::unique_ptr<Application> Application::createApplication(gfx::API api){
	#if CLV_PLATFORM_WINDOWS
		return std::make_unique<WindowsApplication>(api);
	#elif CLV_PLATFORM_LINUX
		return std::make_unique<LinuxApplication>(api);
	#elif CLV_PLATFORM_MACOS
		return std::make_unique<MacApplication>(api);
	#endif
	}

	gfx::API Application::getPreferedAPI(){
	#if CLV_PLATFORM_WINDOWS
		return gfx::API::DirectX11;
	#elif CLV_PLATFORM_LINUX
		return gfx::API::OpenGL4;
	#elif CLV_PLATFORM_MACOS
		return gfx::API::Metal1;
	#endif
	}
}
