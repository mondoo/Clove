#include "EditorSession.hpp"

#include "Layer.hpp"

namespace Bulb::Core{
	public ref class LayerTest : Layer{
	private:
		Entity^ cubeEntity;
		Entity^ camEntity;

		int width;
		int height;

	public:
		LayerTest(int vpWidth, int vpHeight){
			width = vpWidth;
			height = vpHeight;
		}

		void onAttach() override{
			cubeEntity = Application::createEntity();
			cubeEntity->makeCubeEnt();

			camEntity = Application::createEntity();
			camEntity->makeCamEnt(width, height);
		}

		void onUpdate(float deltaTime) override{
			//Console.WriteLine("Updated");
		}

		void onDetach() override{
			//Console.WriteLine("Detached");
		}
	};

	void EditorSession::Begin(System::IntPtr hWnd, int width, int height){
		app = gcnew Application(hWnd, width, height);
		
		app->start();
		
		app->pushLayer(gcnew LayerTest(width, height));
		
		appThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorSession::Update));
		appThread->Start();
	}

	void EditorSession::Update(){
		while(app->shouldRun()){
			app->update();
		}
	}

	void EditorSession::End(){
		app->stop();
		appThread->Join();
	}
}