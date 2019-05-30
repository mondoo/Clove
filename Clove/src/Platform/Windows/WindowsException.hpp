#pragma once

#include "Clove/Exception/CloveException.hpp"

namespace clv{
	class WindowsException : public CloveException{
		//VARIABLES
	private:
		HRESULT hr;

		//FUNCTIONS
	public:
		WindowsException() = delete;
		WindowsException(const WindowsException& other) = delete;
		WindowsException(WindowsException&& other) noexcept = delete;

		WindowsException(int lineNum, const char* file, HRESULT hr);

		virtual ~WindowsException();

		virtual const char* what() const noexcept override;
		inline virtual const char* getType() const noexcept override;

		inline HRESULT getErrorCode() const noexcept;
		inline std::string getErrorString() const noexcept;

		static std::string translateErrorCode(HRESULT hr) noexcept;

		WindowsException& operator=(const WindowsException& other) = delete;
		WindowsException& operator=(WindowsException&& other) noexcept = delete;
	};
}

#include "WindowsException.inl"

#define CLV_WINDOWS_EXCEPTION(hr) WindowsException(__LINE__, __FILE__, hr)
#define CLV_WINDOWS_LAST_EXCEPTION WindowsException(__LINE__, __FILE__, GetLastError())