#pragma once

namespace clv::plt{
	class Application;
}

namespace clv::blb{
	class EditorLayer;
}

namespace Bulb::Core{
	public ref class EditorSession{
		//VARIABLES
	private:
		clv::plt::Application* app = nullptr;
		System::Threading::Thread^ appThread;

		//Putting the layer in here for now so the window can control it
		std::shared_ptr<clv::blb::EditorLayer>* layer;

		//FUNCTIONS
	public:
		!EditorSession();

		void Begin(System::IntPtr hWnd, int posX, int posY, int width, int height);
		void Update();
		void End();

		//Temp
		void AddEntityToLayer();
	};
}