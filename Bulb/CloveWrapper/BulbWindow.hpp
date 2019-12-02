#pragma once

#include <Clove/Core/Platform/Window.hpp>

namespace Clove{
	//Copied from WindowsWindow
	struct WindowsData{
		HWND handle;
		clv::uint32 width;
		clv::uint32 height;
	};
}

namespace Clove{
	class BulbWindow : public clv::plt::Window{
		//VARIABLES
	private:
		HWND handle;
		WindowsData data;

		//FUNCTIONS
	public:
		BulbWindow(System::IntPtr hWnd, int width, int height);

		virtual void* getNativeWindow() const override;

	protected:
		virtual void processInput() override{ /* Empty for now */ }
	};
}