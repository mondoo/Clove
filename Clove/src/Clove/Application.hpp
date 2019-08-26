#pragma once

#include "Clove/ECS/Manager.hpp"
//Audio test
#include "Clove/Audio/SoundPlayer.hpp"
//

namespace clv::gfx{
	class Renderer;
}

namespace clv::evt{
	enum class HandledType;
}

namespace clv{
	class Window;
	class LayerStack;
	class Layer;
	class ImGuiLayer;

	class Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<ecs::Manager> ecsManager;

		bool running = true;

		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//Audio test
		aud::SoundPlayer player;
		//

		//FUNCTIONS
	public:
		Application();
		virtual ~Application();

		void run();
		void stop();

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		Window& getWindow();
		ecs::Manager& getManager();

	private:
		void onWindowClose();
	};

	//To be defined in CLIENT
	Application* createApplication();
}