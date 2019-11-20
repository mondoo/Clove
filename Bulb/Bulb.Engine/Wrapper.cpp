#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include <Windows.h>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <optional>

//Includes required when not using pch
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
		clv::gfx::RenderCommand::makeSurfaceCurrent(*surface);
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

void Bulb::Engine::Wrapper::OpenClove(IntPtr hWnd){
	app = new EditorApplication(hWnd);
	
	app->start();

	//TODO: Push some layers on

	while(app->getState() == clv::plt::ApplicationState::running){
		app->update();
	}

	delete app;
	app = nullptr;
}

void Bulb::Engine::Wrapper::StopClove(){
	app->stop();
}
