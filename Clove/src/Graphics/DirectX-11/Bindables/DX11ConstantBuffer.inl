#include "Core/Application.hpp"
#include "Core/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>

namespace clv::gfx{
	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept = default;

	template<typename T>
	DX11ConstantBuffer<T>& DX11ConstantBuffer<T>::operator=(DX11ConstantBuffer&& other) noexcept = default;

	template<typename T>
	DX11ConstantBuffer<T>::~DX11ConstantBuffer() = default;

	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer(uint32 bindingPoint)
		: bindingPoint(bindingPoint){

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateBuffer(&cbd, nullptr, &constantBuffer));
	}

	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer(uint32 bindingPoint, T&& data)
		: bindingPoint(bindingPoint){

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA csrd = { };
		csrd.pSysMem = &data;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateBuffer(&cbd, &csrd, &constantBuffer));
	}

	template<typename T>
	void DX11ConstantBuffer<T>::update(T&& data){

		D3D11_MAPPED_SUBRESOURCE msr = { };
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getContext().Map(
			constantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		));
		memcpy(msr.pData, &data, sizeof(T));
		DX11RenderAPI::getContext().Unmap(constantBuffer.Get(), 0u);
	}

	template<typename T>
	void DX11VertexConstantBuffer<T>::bind(){
		DX11RenderAPI::getContext().VSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}

	template<typename T>
	void DX11PixelConstantBuffer<T>::bind(){
		DX11RenderAPI::getContext().PSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}

	template<typename T>
	void DX11GeometryConstantBuffer<T>::bind(){
		DX11RenderAPI::getContext().GSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}
}