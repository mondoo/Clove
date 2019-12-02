#pragma once

/*
Temp wrapper until more objects have been made
*/

namespace clv::plt{
	class Application;
}

namespace Bulb::Engine::Clove{
	public ref class Wrapper {
	private:
		clv::plt::Application* app;
	public:
		void OpenClove(System::IntPtr hWnd, int width, int height);

		void StopClove();
	};
}