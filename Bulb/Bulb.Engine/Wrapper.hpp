#pragma once

namespace clv{
	class Application;
}

namespace Bulb::Engine{
	public ref class Wrapper {
	private:
		clv::Application* app;
	public:
		void OpenClove();
		void OpenClove(System::IntPtr hWnd);

		void StopClove();
	};
}