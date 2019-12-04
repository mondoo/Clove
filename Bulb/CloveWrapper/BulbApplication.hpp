#pragma once

#include "BulbLayer.hpp"

#include <Clove/Core/Platform/Application.hpp>

namespace clv::plt::blb{
	class BulbNativeApplication : public clv::plt::Application{
		//VARIABLES
	private:
		System::IntPtr hWnd;
		int width = 0;
		int height = 0;

		//FUNCTIONS
	public:
		BulbNativeApplication(System::IntPtr hWnd, int width, int height);

		virtual clv::gfx::API getPlatformPreferedAPI() override;

		virtual std::unique_ptr<clv::plt::Window> createWindow(const clv::plt::WindowProps& props);
	};
}

namespace Bulb::CloveWrapper{
	public ref class BulbApplication{
		//VARIABLES
	private:
		clv::plt::blb::BulbNativeApplication* nativeApp = nullptr;

		//NOTE: Storing the managed layers in here to stop gc
		System::Collections::Generic::List<BulbLayer^> layers;

		//FUNCTIONS
	public:
		//TODO: Ctors

		BulbApplication(System::IntPtr hWnd, int width, int height);
		~BulbApplication();

		void start();
		void update();
		void stop();

		bool shouldRun();

		void pushLayer(BulbLayer^ layer);
		//void pushOverlay(std::shared_ptr<Layer> overlay); //TODO
	};
}