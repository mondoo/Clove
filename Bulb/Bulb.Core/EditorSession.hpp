#pragma once

#include "Entity.hpp"

namespace tnc{
	class Application;
}

namespace clv::plt{
	class Window;
}

namespace clv::blb{
	class EditorLayer;
}

namespace Bulb::Core{
	public ref class EditorSession{
		//VARIABLES
	private:
		tnc::Application* app = nullptr;
		std::shared_ptr<clv::plt::Window>* window = nullptr;

		System::Threading::Thread^ appThread;

		//Putting the layer in here for now so the window can control it
		std::shared_ptr<clv::blb::EditorLayer>* layer;

		//FUNCTIONS
	public:
		!EditorSession();

		void Begin(System::IntPtr hWnd, int posX, int posY, int width, int height);
		void Update();
		void End();

		//Temp - Just to get functionality working
		Entity^ AddEntityToLayer();
		void UpdateWindowSize(int sizeX, int sizeY);
		void UpdateWindowPosition(int x, int y);
	};
}