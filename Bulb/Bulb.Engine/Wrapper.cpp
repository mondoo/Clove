#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include <Windows.h>
#include <unordered_map>
#include <functional>
#include <sstream>

//Includes required when not using pch
#include "Core/Maths/MathsTypes.hpp"
#include "Core/Maths/Maths.hpp"
#include "Core/Maths/MathsHelpers.hpp"
#include "Core/Application.hpp"
#include "Core/IntTypes.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/Context.hpp"

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

//Create the context with the windows data
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

		context = clv::gfx::Context::createContext(&data, clv::gfx::API::DirectX11);
		context->makeCurrent();
	}

	virtual void* getNativeWindow() const override{
		return handle;
	}

protected:
	virtual void processInput() override{
		//Empty for now
	}
};

void Bulb::Engine::Wrapper::OpenClove(IntPtr hWnd){
	std::unique_ptr<EditorWindow> window = std::make_unique<EditorWindow>(hWnd);
	app = new clv::Application(std::move(window));
	
	while(app->getState() == clv::ApplicationState::running){
		app->update();
	}

	delete app;
	app = nullptr;
}

void Bulb::Engine::Wrapper::StopClove(){
	app->stop();
}
