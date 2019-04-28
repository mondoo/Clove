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

		const char* what() const noexcept override;

		virtual const char* getType() const noexcept override;

		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;

		static std::string translateErrorCode(HRESULT hr) noexcept;

		WindowsException& operator=(const WindowsException& other) = delete;
		WindowsException& operator=(WindowsException&& other) noexcept = delete;
	};
}
