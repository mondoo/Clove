#pragma once

#include "Application.hpp"

namespace Bulb::Core{
	public ref class EditorSession{
		//VARIABLES
	private:
		Application^ app;
		System::Threading::Thread^ appThread;

		//FUNCTIONS
	public:
		void Begin(System::IntPtr hWnd, int width, int height);
		void Update();
		void End();
	};
}