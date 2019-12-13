#pragma once

#include "EditorApplication.hpp"

namespace Bulb::Core{
	public ref class EditorSession{
		//VARIABLES
	private:
		clv::plt::blb::EditorApplication* app;
		System::Threading::Thread^ appThread;

		//FUNCTIONS
	public:
		!EditorSession();

		void Begin(System::IntPtr hWnd, int width, int height);
		void Update();
		void End();
	};
}