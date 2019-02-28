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

void registerWindowClass(HINSTANCE hIsnt, const wchar_t* windowClassName){
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = &wndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hIsnt;
	windowClass.hIcon = ::LoadIcon(hIsnt, NULL);
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = ::LoadIcon(hIsnt, NULL);

	static ATOM atom = ::RegisterClassExW(&windowClass);
	assert(atom > 0);
}

HWND createWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height){
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	//Center window within the screen. clamp 0, 0, for the top left corner
	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

	HWND hWnd = ::CreateWindowExW(
		NULL,
		windowClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		hInst,
		nullptr
	);

	assert(hWnd && "Failed to create window");

	return hWnd;
}

ComPtr<IDXGIAdapter4> getAdapter(bool useWarp){
	ComPtr<IDXGIFactory4> dxgiFactory;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	ComPtr<IDXGIAdapter1> dxgiAdapter1;
	ComPtr<IDXGIAdapter4> dxgiAdapter4;

	if(useWarp){
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
		ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
	} else{
		SIZE_T maxDedicatedVideoMemory = 0;
		for(UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i){
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without actually 
			// creating it. The adapter with the largest dedicated video memory
			// is favored.
			if((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && 
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory){
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}

	return dxgiAdapter4;
}

ComPtr<ID3D12Device2> createDevice(ComPtr<IDXGIAdapter4> adapter){
	ComPtr<ID3D12Device2> d3d12Device2;
	ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));

	//Enable debug messages in debug mode
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if(SUCCEEDED(d3d12Device2.As(&pInfoQueue))){
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

		//Suppress whole categories of messages
		//D3D12_MESSAGE_CATEGORY categories[] = {};

		//Suppress messages based on their severit level
		D3D12_MESSAGE_SEVERITY severities[] = {
			D3D12_MESSAGE_SEVERITY_INFO
		};

		//Suppress individual messages byu their ID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,							// This warning occurs when using capture frame while graphics debugging.
			D3D12_MESSAGE_ID_UNMAP_NULLRANGE								// This warning occurs when using capture frame while graphics debugging.
		};

		D3D12_INFO_QUEUE_FILTER newFilter = {};
		//newFilter.DenyList.NumCategories = _countof(categories);
		//newFilter.DenyList.pCategoryList = categories;
		newFilter.DenyList.NumSeverities = _countof(severities);
		newFilter.DenyList.pSeverityList = severities;
		newFilter.DenyList.NumIDs = _countof(denyIds);
		newFilter.DenyList.pIDList = denyIds;

		ThrowIfFailed(pInfoQueue->PushStorageFilter(&newFilter));
	}
#endif // defined(_DEBUG)

	return d3d12Device2;
}

//at: Create the Command Queue