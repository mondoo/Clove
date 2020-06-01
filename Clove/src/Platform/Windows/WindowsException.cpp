#include "Clove/Platform/Windows/WindowsException.hpp"

namespace clv{
	WindowsException::WindowsException(int lineNum, const char* file, HRESULT hr)
		: CloveException(lineNum, file)
		, hr(hr){
	}

	WindowsException::~WindowsException() = default;

	const char* WindowsException::what() const noexcept{
		std::ostringstream oss;
		oss << getType() << std::endl
			<< "[ERROR CODE] " << getErrorCode() << std::endl
			<< "[DESCRIPTION] " << getErrorString() << std::endl
			<< getOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* WindowsException::getType() const noexcept{
		return "Clove Windows Exception";
	}

	HRESULT WindowsException::getErrorCode() const noexcept{
		return hr;
	}

	std::string WindowsException::getErrorString() const noexcept{
		return translateErrorCode(hr);
	}

	std::string WindowsException::translateErrorCode(HRESULT hr) noexcept{
		wchar_t* msgBuff = nullptr;
		DWORD msgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&msgBuff),
			0,
			nullptr
		);

		if(msgLen == 0){
			return "Unidentified error code";
		}

		std::wstring wideString = msgBuff;
		std::string string;
		int32_t sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wideString[0], static_cast<int32_t>(wideString.size()), nullptr, 0, nullptr, nullptr);

		if(sizeNeeded <= 0){
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Was unable to convert wide char to multi byte");
			GARLIC_LOG(garlicLogContext, Log::Level::Error, GetLastError());
		} else{
			WideCharToMultiByte(CP_UTF8, 0, &wideString[0], static_cast<int32_t>(wideString.size()), &string[0], sizeNeeded, nullptr, nullptr);
		}

		LocalFree(msgBuff);
		return string;
	}
}