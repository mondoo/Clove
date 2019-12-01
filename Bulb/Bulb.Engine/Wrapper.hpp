#pragma once

namespace clv::plt{
	class Application;
}

namespace Bulb::Engine{
	public ref class Wrapper {
	private:
		clv::plt::Application* app;
	public:
		void OpenClove(System::IntPtr hWnd, int width, int height);

		void StopClove();
	};
}