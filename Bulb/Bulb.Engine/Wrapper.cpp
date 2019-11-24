#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include <Windows.h>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <optional>
#include <array>

//Includes required when not using pch
#include "Core/Core.hpp"
#include "Core/IntTypes.hpp"
#include "Core/Utils/Delegate.hpp"
#include "Core/Maths/MathsTypes.hpp"
#include "Core/Maths/Maths.hpp"
#include "Core/Maths/MathsHelpers.hpp"
#include "Core/Platform/Application.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/Surface.hpp"
#include "Core/Graphics/RenderCommand.hpp"

//Defining this here to fix errors - need a way to get access to logging (or sort out headers)
#define CLV_LOG_ERROR(...)

//Other includes
#include "Core/Layer.hpp"
#include "Core/ECS/Manager.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/MeshComponent.hpp"
#include "Core/ECS/3D/Components/CameraComponent.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/Material.hpp"
#include "Core/Graphics/MaterialInstance.hpp"

using namespace System::Windows;
//using namespace System::Runtime::RuntimeInteropServices;
using namespace System::Threading;
using namespace System;

//Copied from WindowsWindow
struct WindowsData{
	HWND handle;
	clv::uint32 width;
	clv::uint32 height;
};

//Temp window for the editor
class EditorWindow : public clv::plt::Window{
	//VARIABLES
private:
	HWND handle;
	WindowsData data;

	//FUNCTIONS
public:
	EditorWindow(IntPtr hWnd){
		handle = reinterpret_cast<HWND>(hWnd.ToPointer());

		data = { handle, 800, 400 };//Hard coded values from the xml

		surface = clv::gfx::RenderCommand::createSurface(&data);
		clv::gfx::RenderCommand::makeSurfaceCurrent(surface);
	}

	virtual void* getNativeWindow() const override{
		return handle;
	}

protected:
	virtual void processInput() override{
		//Empty for now
	}
};

//Temp application for the editor
class EditorApplication : public clv::plt::Application{
	//VARIABLES
private:
	IntPtr hWnd;

	//FUNCTIONS
public:
	EditorApplication(IntPtr hWnd){ //Temp: Just passing in the handle from the wpf app
		this->hWnd = hWnd;
	}

	virtual clv::gfx::API getPlatformPreferedAPI() override{
		return clv::gfx::API::DirectX11;
	}

	virtual std::unique_ptr<clv::plt::Window> createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<EditorWindow>(hWnd);
	}
};

//Temp layer to draw something
class TestLayer : public clv::Layer{
	//VARIABLES
private:
	clv::ecs::Entity cubeEnt;
	clv::ecs::Entity camEnt;

	//FUNCTIONS
public:
	virtual void onAttach() override{
		auto cubeMaterial = std::make_shared<clv::gfx::Material>();
		cubeMaterial->setData(clv::gfx::BBP_Colour, clv::mth::vec4f{ 0.4f, 0.4f, 0.4f, 1.0f }, clv::gfx::ShaderType::Pixel);
		cubeMaterial->setData(clv::gfx::BBP_MaterialData, clv::gfx::MaterialData{ 32.0f }, clv::gfx::ShaderType::Pixel);

		auto inst = cubeMaterial->createInstance();
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", inst); //NOTE: Manually copied to the correct folder for now

		cubeEnt = clv::plt::Application::get().getManager().createEntity();
		cubeEnt.addComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 10.0f, 0.0f, 0.0f });
		cubeEnt.addComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);


		camEnt = clv::plt::Application::get().getManager().createEntity();
		camEnt.addComponent<clv::ecs::_3D::TransformComponent>();
		camEnt.addComponent<clv::ecs::_3D::CameraComponent>(clv::gfx::Viewport{ 0, 0, 800, 400 }); //Any old viewport for now
	}

	virtual void onDetach() override{
	
	}

	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override{
		static float rotDelta = 0.0f;
		const float rotSpeed = 1.5f;

		cubeEnt.getComponent<clv::ecs::_3D::TransformComponent>()->setRotation(clv::mth::asQuaternion(rotDelta, clv::mth::vec3f{ 0.0f, 1.0f, 0.0f }));

		rotDelta += rotSpeed * deltaTime;
	}
};

void Bulb::Engine::Wrapper::OpenClove(IntPtr hWnd){
	app = new EditorApplication(hWnd);
	
	app->start();

	app->pushLayer(std::make_shared<TestLayer>());

	while(app->getState() == clv::plt::ApplicationState::running){
		app->update();
	}

	delete app;
	app = nullptr;
}

void Bulb::Engine::Wrapper::StopClove(){
	app->stop();
}
