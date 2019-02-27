#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> //For CommandLineToArgvW

//min/max may conflict with std::min/std::max
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

//We want to create a function named CreateWindow, so remove this one
#if defined(CreateWindow)
#undef CreateWindow
#endif

//Gives us ComPtr - smart pointers for com objects
#include <wrl.h>
using namespace Microsoft::WRL;

//DirectX 12 headers
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//Extension library, provides classes that will simplify functions
#include "d3dx12.h"

//STL headers
#include <algorithm>
#include <cassert>
#include <chrono>

//Helpers
#include "Helpers.h"

//Number of swap chain back buffers
const uint8_t numFrames = 3;
//Use WARP adapter
bool useWarp = false;

uint32_t clientWidth = 1280;
uint32_t clientHeight = 800;

//Set to true once the DX12 objects have been initialised
bool isInitialized = false;

//Window handle
HWND hWnd;
//Window rectangle (used to toggle fullscreen state)
RECT windowRect;

//DirectX 12 Objects
ComPtr<ID3D12Device2> device;
ComPtr<ID3D12CommandQueue> commandQueue;
ComPtr<IDXGISwapChain4> swapChain;
ComPtr<ID3D12Resource> backBuffers[numFrames];
ComPtr<ID3D12GraphicsCommandList> commandList; //Will need more for more threads
ComPtr<ID3D12CommandAllocator> commandAllocators[numFrames]; //Puts commands into the command list, can only swap once all commands are processed
ComPtr<ID3D12DescriptorHeap> RTVDescriptorHeap; //Stores the RTVs for the swap chain back buffers
UINT RTVDescriptorSize;
UINT currentBackBufferIndex;

//Synchronisation objects
ComPtr<ID3D12Fence> fence;
uint64_t fenceValue = 0;
uint64_t frameFenceValues[numFrames] = {};
HANDLE fenceEvent;

//By default, enable v-sync
//can be toggled with the V key
bool VSync = true;
bool tearingSuppored = false;
//By default, used windowed mode
//Can be toggled with Alt+Enter or F11
bool fullScreen = false;

//Window callback function
LRESULT	CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

void parseCommandLineArguments(){
	int argc;
	wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

	for(size_t i = 0; i < argc; i++){
		if(::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0){
			clientWidth = ::wcstol(argv[++i], nullptr, 10);
		}
		if(::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0){
			clientHeight = ::wcstol(argv[++i], nullptr, 10);
		}
		if(::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0){
			useWarp = true;
		}
	}

	::LocalFree(argv);
}

void enableDebugLayer(){
#if defined(_DEBUG)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif
}

//Stopped on:

/*
The IID_PPV_ARGS macro shown here on line 118 is used to retrieve an interface pointer, supplying the IID value of the requested interface automatically based on the type of the interface pointer used.
A common syntax in methods that retrieve an interface pointer includes two parameters:

	An [in] parameter, normally of type REFIID, to specify the IID of the interface to retrieve.
	An [out] parameter, normally of type void**, to receive the interface pointer.

This macro computes the IID based on the type of interface pointer, which prevents coding errors in which the IID and interface pointer type do not match. Windows developers should always use this macro with any method that requires separate IID and interface pointer parameters.

*/