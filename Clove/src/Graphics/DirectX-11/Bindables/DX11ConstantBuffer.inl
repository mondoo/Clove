#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"

#include <d3d11.h>

namespace clv::gfx{
	template<typename T>
	inline DX11ConstantBuffer<T>::DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept = default;

	template<typename T>
	inline DX11ConstantBuffer<T>& DX11ConstantBuffer<T>::operator=(DX11ConstantBuffer&& other) noexcept = default;

	template<typename T>
	inline DX11ConstantBuffer<T>::~DX11ConstantBuffer() = default;

	template<typename T>
	inline DX11ConstantBuffer<T>::DX11ConstantBuffer(unsigned int bindingPoint)
		: bindingPoint(bindingPoint){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&Application::get().getRenderer());
		DX11_INFO_PROVIDER(dxrenderer);

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0;

		DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&cbd, nullptr, &constantBuffer));
	}

	template<typename T>
	inline DX11ConstantBuffer<T>::DX11ConstantBuffer(unsigned int bindingPoint, const T& data)
		: bindingPoint(bindingPoint){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&Application::get().getRenderer());
		DX11_INFO_PROVIDER(dxrenderer);

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(data);
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA csrd = { };
		csrd.pSysMem = &data;

		DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&cbd, &csrd, &constantBuffer));
	}

	template<typename T>
	inline void DX11ConstantBuffer<T>::update(const T& data, Renderer& renderer){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&renderer);
		DX11_INFO_PROVIDER(dxrenderer);

		D3D11_MAPPED_SUBRESOURCE msr = { };
		DX11_THROW_INFO(dxrenderer->getContext().Map(
			constantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		));
		memcpy(msr.pData, &data, sizeof(data));
		dxrenderer->getContext().Unmap(constantBuffer.Get(), 0u);
	}

	template<typename T>
	inline void DX11VertexConstantBuffer<T>::bind(Renderer& renderer){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&renderer);
		dxrenderer->getContext().VSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}

	template<typename T>
	inline void DX11VertexConstantBuffer<T>::unbind(){
	}

	template<typename T>
	inline void DX11PixelConstantBuffer<T>::bind(Renderer& renderer){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&renderer);
		dxrenderer->getContext().PSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}

	template<typename T>
	inline void DX11PixelConstantBuffer<T>::unbind(){
	}
}