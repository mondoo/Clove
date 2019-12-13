#include "EditorSession.hpp"

#include "EditorLayer.hpp"

namespace Bulb::Core{
	EditorSession::!EditorSession(){
		End();
	}

	void EditorSession::Begin(System::IntPtr hWnd, int width, int height){
		app = new clv::plt::blb::EditorApplication(hWnd, width, height);
		
		app->start();
		
		app->pushLayer(std::make_shared<clv::blb::EditorLayer>());
		
		appThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorSession::Update));
		appThread->Start();
	}

	void EditorSession::Update(){
		while(app->getState() == clv::plt::ApplicationState::running){
			app->update();
		}
	}

	void EditorSession::End(){
		app->stop();
		appThread->Join();

		delete app;
	}
}