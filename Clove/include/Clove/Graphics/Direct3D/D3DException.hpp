#pragma once

#include "Clove/Exception/CloveException.hpp"

#include "Clove/Graphics/Direct3D/DXGIInfoManager.hpp"

namespace clv::gfx::d3d{
	class D3DException : public CloveException{
		//VARIABLES
	public:
		HRESULT hr;
		std::string info;

		//FUNCTIONS
	public:
		D3DException() = delete;
		D3DException(const D3DException& other) = delete;
		D3DException(D3DException&& other) noexcept = delete;
		CloveException& operator=(const CloveException& other) = delete;
		CloveException& operator=(CloveException&& other) noexcept = delete;
		virtual ~D3DException();

		D3DException(int32_t lineNum, const char* file, HRESULT hr, const std::vector<std::string>& messages = {});

		virtual const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;

		HRESULT getErrorCode() const noexcept;

		std::string getErrorString() const noexcept;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	};

	class DeviceRemovedException : public D3DException{
		using D3DException::D3DException;

		//VARIABLES
	private:
		std::string reason;

		//FUNCTIONS
	public:
		DeviceRemovedException() = delete;
		DeviceRemovedException(const DeviceRemovedException& other) = delete;
		DeviceRemovedException(DeviceRemovedException&& other) noexcept = delete;
		DeviceRemovedException& operator=(const DeviceRemovedException& other) = delete;
		DeviceRemovedException& operator=(DeviceRemovedException&& other) noexcept = delete;
		~DeviceRemovedException();

		virtual const char* getType() const noexcept override;
	};

	class InfoException : public CloveException{
		//VARIABLES
	private:
		std::string info;

	#if GARLIC_DEBUG
		static DXGIInfoManager infoManager;
	#endif

		//FUNCTIONS
	public:
		InfoException() = delete;
		InfoException(const InfoException& other) = delete;
		InfoException(InfoException&& other) noexcept = delete;
		InfoException& operator=(const InfoException& other) = delete;
		InfoException& operator=(InfoException&& other) noexcept = delete;
		~InfoException();

		InfoException(int32_t lineNum, const char* file, const std::vector<std::string>& messages);

		virtual const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;

		std::string getErrorInfo() const noexcept;

	#if GARLIC_DEBUG
		static DXGIInfoManager& getInfoManager();
	#endif
	};
}

#define DX11_EXCEPT_NOINFO(hr) clv::gfx::d3d::D3DException(__LINE__, __FILE__, (hr))
#define DX11_THROW_NOINFO(hrcall) { if(FAILED(hr = (hrcall))){ throw DX11_EXCEPT_NOINFO(hr); } }

#if GARLIC_DEBUG
	#define DX11_EXCEPT(hr) clv::gfx::d3d::D3DException(__LINE__, __FILE__, (hr), infoManager.getMessages())
	#define DX11_THROW_INFO(hrcall) infoManager.set(); if(FAILED(hr = (hrcall))) throw DX11_EXCEPT(hr)
	#define DX11_DEVICE_REMOVED_EXCPTION(hr) clv::gfx::d3d::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.getMessages())
	#define DX11_THROW_INFO_ONLY(call) infoManager.set(); (call); { auto v = infoManager.getMessages(); if(!v.empty()){ throw clv::gfx::d3d::InfoException(__LINE__, __FILE__, v); } }

	#define DX11_INFO_PROVIDER HRESULT hr; DXGIInfoManager& infoManager = InfoException::getInfoManager();
#else
	#define DX11_EXCEPT(hr) clv::gfx::d3d::D3DException(__LINE__, __FILE__, (hr))
	#define DX11_THROW_INFO(hrcall) DX11_THROW_NOINFO(hrcall)
	#define DX11_DEVICE_REMOVED_EXCPTION(hr) clv::gfx::d3d::DeviceRemovedException(__LINE__, __FILE__, (hr))
	#define DX11_THROW_INFO_ONLY(call) (call)

	#define DX11_INFO_PROVIDER HRESULT hr;
#endif