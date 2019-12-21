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
#include "Clove/Core/Core.hpp"
#include "Clove/Core/IntTypes.hpp"
#include "Clove/Core/Utils/Delegate.hpp"
#include "Clove/Core/Maths/MathsTypes.hpp"
#include "Clove/Core/Maths/Maths.hpp"
#include "Clove/Core/Maths/MathsHelpers.hpp"
#include "Clove/Core/Platform/Application.hpp"
#include "Clove/Core/Platform/Window.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"
#include "Clove/Core/Graphics/Surface.hpp"
#include "Clove/Core/Graphics/RenderCommand.hpp"

//Defining this here to fix errors - need a way to get access to logging (or sort out headers)
#define CLV_LOG_ERROR(...)

//Other includes
#include "Clove/Core/Layer.hpp"
#include "Clove/Core/ECS/Manager.hpp"
#include "Clove/Core/ECS/Entity.hpp"
#include "Clove/Core/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/Core/ECS/3D/Components/MeshComponent.hpp"
#include "Clove/Core/ECS/3D/Components/CameraComponent.hpp"
#include "Clove/Core/Graphics/Renderables/Mesh.hpp"
#include "Clove/Core/Graphics/Material.hpp"
#include "Clove/Core/Graphics/MaterialInstance.hpp"

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
	EditorWindow(IntPtr hWnd, int width, int height){
		handle = reinterpret_cast<HWND>(hWnd.ToPointer());

		data = { handle, static_cast<clv::uint32>(width), static_cast<clv::uint32>(height) };

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
	int width = 0;
	int height = 0;

	//FUNCTIONS
public:
	EditorApplication(IntPtr hWnd, int width, int height){ //Temp: Just passing in the handle from the wpf app
		this->hWnd = hWnd;
		this->width = width;
		this->height = height;
	}

	virtual clv::gfx::API getPlatformPreferedAPI() override{
		return clv::gfx::API::DirectX11;
	}

	virtual std::unique_ptr<clv::plt::Window> createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<EditorWindow>(hWnd, width, height);
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
		cubeEnt.addComponent<clv::ecs::_3D::MeshComponent>(mesh);


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

void Bulb::Engine::Wrapper::OpenClove(IntPtr hWnd, int width, int height){
	app = new EditorApplication(hWnd, width, height);
	
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
