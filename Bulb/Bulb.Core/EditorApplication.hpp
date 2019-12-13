#pragma once

#include <Clove/Core/Platform/Application.hpp>

namespace Bulb::Core{
	ref class Layer;
}

namespace clv::plt::blb{
	class EditorApplication : public clv::plt::Application{
		//VARIABLES
	private:
		System::IntPtr hWnd;
		int width = 0;
		int height = 0;

		//FUNCTIONS
	public:
		EditorApplication(System::IntPtr hWnd, int width, int height);

		virtual clv::gfx::API getPlatformPreferedAPI() override;

		virtual std::unique_ptr<clv::plt::Window> createWindow(const clv::plt::WindowProps& props);
	};
}