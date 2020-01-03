#include "EditorSession.hpp"

#include "EditorLayer.hpp"
#include "EditorWindowProxy.hpp"

#include <Clove/Core/Platform/Application.hpp>
#include <msclr/lock.h>

namespace Bulb::Core{
	EditorSession::!EditorSession(){
		End();
	}

	void EditorSession::Begin(System::IntPtr hWnd, int posX, int posY, int width, int height){
		//TODO: Support macOS / Linux windows

		window = new std::shared_ptr<clv::plt::Window>();

		auto appUniquePtr = clv::plt::Application::createApplication(clv::gfx::API::DirectX11);
		app = appUniquePtr.release();
		
		clv::plt::blb::EditorWindowProxy proxy = { hWnd };

		*window = app->openChildWindow(clv::plt::WindowType::MainWindow, proxy, { posX, posY }, { width, height });
		app->setMainWindow(*window);

		layer = new std::shared_ptr(std::make_shared<clv::blb::EditorLayer>());
		app->pushLayer(*layer);
		
		appThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorSession::Update));
		appThread->Start();
	}

	void EditorSession::Update(){
		while(app->getState() == clv::plt::ApplicationState::running){
			msclr::lock l(appThread);
			app->update();
		}
	}

	void EditorSession::End(){
		if(app){
			{
				msclr::lock l(appThread);
				app->stop();
			}
			appThread->Join();

			delete app;
			app = nullptr;
		}

		if(window){
			delete window;
			window = nullptr;
		}

		if(layer){
			delete layer;
			layer = nullptr;
		}
	}

	Entity^ EditorSession::AddEntityToLayer(){
		msclr::lock l(appThread);
		return gcnew Entity((*layer)->addEntity().getID());
	}

	void EditorSession::UpdateWindowSize(int sizeX, int sizeY){
		if(app){
			msclr::lock l(appThread);
			app->getMainWindow().resizeWindow({ sizeX, sizeY });
		}
	}

	void EditorSession::UpdateWindowPosition(int x, int y){
		if(app){
			msclr::lock l(appThread);
			app->getMainWindow().moveWindow({ x, y });
		}
	}
}