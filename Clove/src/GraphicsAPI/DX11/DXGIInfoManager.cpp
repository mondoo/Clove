#include "clvpch.hpp"
#include "DXGIInfoManager.hpp"

#include "GraphicsAPI/DX11/DX11Exception.hpp"
#include "Platform/Windows/WindowsException.hpp"

#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

namespace clv{
	namespace graphics{
		DXGIInfoManager::DXGIInfoManager(){
			//define function signature of DXGIGetDebugInterface
			typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

			//load the dll that contains the function
			const auto hModDXGIDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
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
}

//TODO: DX11_THROW_FAILED into DX11_THROW_NOINFO