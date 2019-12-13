#pragma once

namespace clv::plt{
	class Application;
}

namespace Bulb::Core{
	public ref class EditorSession{
		//VARIABLES
	private:
		clv::plt::Application* app = nullptr;
		System::Threading::Thread^ appThread;

		//FUNCTIONS
	public:
		!EditorSession();

		void Begin(System::IntPtr hWnd, int posX, int posY, int width, int height);
		void Update();
		void End();
	};
}