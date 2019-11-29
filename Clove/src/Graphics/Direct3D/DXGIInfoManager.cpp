#include "Clove/Graphics/Direct3D/DXGIInfoManager.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Platform/Windows/WindowsException.hpp"

#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

namespace clv::gfx::d3d{
	DXGIInfoManager::DXGIInfoManager(){
		//define function signature of DXGIGetDebugInterface
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		//load the dll that contains the function
		const auto hModDXGIDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if(hModDXGIDebug == nullptr){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		//get the address of the function in the dll
		const auto DXGIDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModDXGIDebug, "DXGIGetDebugInterface"))
			);
		if(DXGIDebugInterface == nullptr){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		HRESULT hr;
		DX11_THROW_NOINFO(DXGIDebugInterface(__uuidof(IDXGIInfoQueue), &DXGIInfoQueue));
	}

	DXGIInfoManager::DXGIInfoManager(DXGIInfoManager&& other) noexcept = default;

	DXGIInfoManager& DXGIInfoManager::operator=(DXGIInfoManager&& other) noexcept = default;

	DXGIInfoManager::~DXGIInfoManager() = default;

	void DXGIInfoManager::set() noexcept{
		next = DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> DXGIInfoManager::getMessages() const{
		std::vector<std::string> messages;
		const auto end = DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for(auto i = next; i < end; ++i){
			HRESULT hr;
			SIZE_T messageLength;

			//get the size of the message i in bytes
			DX11_THROW_NOINFO(DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

			//allocate memory for the message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

			//get the message and push its description into the vector
			DX11_THROW_NOINFO(DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength));
			messages.emplace_back(message->pDescription);
		}
		return messages;
	}
}