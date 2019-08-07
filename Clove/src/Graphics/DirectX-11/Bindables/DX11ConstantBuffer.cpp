#include "clvpch.hpp"
#include "DX11ConstantBuffer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11ConstantBuffer::DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept = default;

	DX11ConstantBuffer& DX11ConstantBuffer::operator=(DX11ConstantBuffer&& other) noexcept = default;

	DX11ConstantBuffer::~DX11ConstantBuffer() = default;

	DX11ConstantBuffer::DX11ConstantBuffer(unsigned int bindingPoint)
		: bindingPoint(bindingPoint){

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T); //fok - will need to take some arbitrary size
		cbd.StructureByteStride = 0;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateBuffer(&cbd, nullptr, &constantBuffer));
	}

	//This is the constructor that isn't used
	DX11ConstantBuffer::DX11ConstantBuffer(unsigned int bindingPoint, const MaterialData& data)
		: bindingPoint(bindingPoint){

		D3D11_BUFFER_DESC cbd = { };
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = data.getSize();
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA csrd = { };
		csrd.pSysMem = data.getBuffer();

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateBuffer(&cbd, &csrd, &constantBuffer));
	}

	void DX11ConstantBuffer::update(const MaterialData& data){

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

	void DX11VertexConstantBuffer::bind(){
		DX11RenderAPI::getContext().VSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}

	void DX11PixelConstantBuffer::bind(){
		DX11RenderAPI::getContext().PSSetConstantBuffers(bindingPoint, 1u, constantBuffer.GetAddressOf());
	}
}