#pragma once

#include "Entity.hpp"

#include <Clove/Core/Platform/Application.hpp>

namespace Bulb::Core{
	ref class Layer;
}

namespace clv::plt::blb{
	class NativeApplication : public clv::plt::Application{
		//VARIABLES
	private:
		System::IntPtr hWnd;
		int width = 0;
		int height = 0;

		//FUNCTIONS
	public:
		NativeApplication(System::IntPtr hWnd, int width, int height);

		virtual clv::gfx::API getPlatformPreferedAPI() override;

		virtual std::unique_ptr<clv::plt::Window> createWindow(const clv::plt::WindowProps& props);
	};
}

namespace Bulb::Core{
	public ref class Application{
		//VARIABLES
	private:
		clv::plt::blb::NativeApplication* nativeApp = nullptr;

		//FUNCTIONS
	public:
		//TODO: Ctors

		Application(System::IntPtr hWnd, int width, int height);
		!Application();

		void start();
		void update();
		void stop();

		bool shouldRun();

		void pushLayer(Layer^ layer);
		//void pushOverlay(std::shared_ptr<Layer> overlay); //TODO

		//TEMP: Having the application create the entity
		static Entity^ createEntity();
	};
}