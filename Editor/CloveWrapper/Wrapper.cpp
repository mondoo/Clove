#include "Wrapper.hpp"

#include <memory>
#include <chrono>

#include <Windows.h>
#include <unordered_map>
#include <functional>
#include <sstream>

#include "Clove/Application.hpp"
#include "Clove/IntTypes.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Context.hpp"

using namespace System::Windows;
//using namespace System::Runtime::RuntimeInteropServices;
using namespace System::Threading;
using namespace System;

class Editor : public clv::Application{
public:
	Editor(){
		
	}
	~Editor(){

	}

private:

};

//Creating an 'Editor Window' that will have the hWnd for the C# window

//Copied from WindowsWindow
struct WindowsData{
	HWND handle;
	clv::uint32 width;
	clv::uint32 height;
};

//Create the context with the windows data
class EditorWindow : public clv::Window{
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
//

//data = { windowsHandle, windowProperties.width, windowProperties.height };

//context = gfx::Context::createContext(&data, api);
//context->makeCurrent();

clv::Application* clv::createApplication(){
	return new Editor;
}

void CloveWrapper::Wrapper::OpenClove(){
	//clv::Log::init();
	app = clv::createApplication();
	app->run();
	delete app;
	app = nullptr;
}

void CloveWrapper::Wrapper::OpenClove(IntPtr hWnd){
	std::unique_ptr<EditorWindow> window = std::make_unique<EditorWindow>(hWnd);
	app = new clv::Application(std::move(window));
	app->run(); //Thread gets stuck here, Clove will need be on another thread than the editor (Bulb)
	delete app;
	app = nullptr;
}

void CloveWrapper::Wrapper::StopClove(){
	app->stop();
}
