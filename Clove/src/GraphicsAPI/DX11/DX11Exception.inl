#include "DX11Exception.hpp"
namespace clv{
	namespace graphics{
		inline const char* DX11Exception::getType() const noexcept{
			return "Clove DX11 Exception";
		}

		inline HRESULT DX11Exception::getErrorCode() const noexcept{
			return hr;
		}

		inline std::string DX11Exception::getErrorInfo() const noexcept{
			return info;
		}

		inline const char* DeviceRemovedException::getType() const noexcept{
			return "Clove DX11 Exception [DEVICE REMOVED] (GXGI_ERROR_DEVICE_REMOVED)";
		}

		inline const char* InfoException::getType() const noexcept{
			return "Clove DX11 Info Exception";
		}

		inline std::string InfoException::getErrorInfo() const noexcept{
			return info;
		}
	}
}