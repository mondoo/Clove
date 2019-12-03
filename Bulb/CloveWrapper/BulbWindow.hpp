#pragma once

#include <Clove/Core/Platform/Window.hpp>

namespace clv::plt{
	//Copied from WindowsWindow
	struct WindowsData{
		HWND handle;
		clv::uint32 width;
		clv::uint32 height;
	};
}

namespace clv::plt::blb{
	class BulbNativeWindow : public clv::plt::Window{
		//VARIABLES
	private:
		HWND handle;
		WindowsData data;

		//FUNCTIONS
	public:
		BulbNativeWindow(System::IntPtr hWnd, int width, int height);

		virtual void* getNativeWindow() const override;

	protected:
		virtual void processInput() override{ /* Empty for now */ }
	};
}

namespace Bulb::CloveWrapper{
	public ref class BulbWindow{
		//VARIABLES
	private:
		clv::plt::blb::BulbNativeWindow* nativeWin = nullptr;

		//FUNCTIONS
	public:
		BulbWindow(System::IntPtr hWnd, int width, int height);
		~BulbWindow();
	};
}