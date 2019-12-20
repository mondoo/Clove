#include "EditorSession.hpp"

#include "EditorLayer.hpp"
#include "EditorWindowProxy.hpp"

#include <Clove/Core/Platform/Application.hpp>

namespace Bulb::Core{
	EditorSession::!EditorSession(){
		End();
	}

	void EditorSession::Begin(System::IntPtr hWnd, int posX, int posY, int width, int height){
		//TODO: Support macOS / Linux windows

		auto appUniqePtr = clv::plt::Application::createApplication();
		app = appUniqePtr.release();
		
		clv::plt::blb::EditorWindowProxy proxy = { hWnd };

		app->initialise(proxy, { posX, posY }, { width, height });
		
		layer = new std::shared_ptr(std::make_shared<clv::blb::EditorLayer>());
		app->pushLayer(*layer);
		
		appThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorSession::Update));
		appThread->Start();
	}

	void EditorSession::Update(){
		while(app->getState() == clv::plt::ApplicationState::running){
			app->update();
		}
	}

	void EditorSession::End(){
		if(app){
			app->stop();
			appThread->Join();

			delete app;
			app = nullptr;
		}

		if(layer){
			delete layer;
			layer = nullptr;
		}
	}

	void EditorSession::AddEntityToLayer(){
		(*layer)->addEntity();
	}

	void EditorSession::UpdateWindowSize(int sizeX, int sizeY){
		if(app){
			/*
			TODO: Find a much better way to do this
			This is a stop gap because DX needs to clear the render target to resize it's buffers.
			This is fine on single thread app but when the application is running on a thread and 
			then told to resize from another thread there is a change it could try and draw to the 
			render target during the resize. This is a TEMPORARY solution to that but can still happen

			Need to probably rethink how the thread is handled or add better handling of race conditions
			*/
			appThread->Suspend(); 
			app->getWindow().resizeWindow({ sizeX, sizeY });
			appThread->Resume();
		}
	}

	void EditorSession::UpdateWindowPosition(int x, int y){
		if(app){
			appThread->Suspend();
			app->getWindow().moveWindow({ x, y });
			appThread->Resume();
		}
	}
}