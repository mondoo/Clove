#include "EditorSession.hpp"

#include "EditorLayer.hpp"
#include "EditorWindowProxy.hpp"

#include <msclr/lock.h>

#include <Tunic/Application.hpp>
#include <Tunic/ECS/Core/World.hpp>
#include <Tunic/ECS/3D/Systems/PhysicsSystem.hpp>
#include <Tunic/ECS/3D/Systems/RenderSystem.hpp>
#include <Tunic/ECS/2D/Systems/PhysicsSystem.hpp>
#include <Tunic/ECS/2D/Systems/RenderSystem.hpp>
#include <Tunic/ECS/Audio/Systems/AudioSystem.hpp>

using namespace tnc;

namespace Bulb::Core{
	EditorSession::!EditorSession(){
		End();
	}

	void EditorSession::Begin(System::IntPtr hWnd, int posX, int posY, int width, int height){
		//TODO: Support macOS / Linux windows

		window = new std::shared_ptr<clv::plt::Window>();

		blb::plt::EditorWindowProxy proxy = { hWnd };

		app = new tnc::Application(clv::gfx::API::Direct3D11, proxy, { posX, posY }, { width, height });

		ecs::World& ecsWorld = app->getWorld();

		ecsWorld.addSystem<ecs::_3D::PhysicsSystem>();
		ecsWorld.addSystem<ecs::_3D::RenderSystem>();

		ecsWorld.addSystem<ecs::_2D::PhysicsSystem>();
		ecsWorld.addSystem<ecs::_2D::RenderSystem>();

		ecsWorld.addSystem<ecs::aud::AudioSystem>();

		layer = new std::shared_ptr(std::make_shared<blb::EditorLayer>());
		app->pushLayer(*layer);
		
		appThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorSession::Update));
		appThread->Start();
	}

	void EditorSession::Update(){
		while(app->getState() == tnc::ApplicationState::running){
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