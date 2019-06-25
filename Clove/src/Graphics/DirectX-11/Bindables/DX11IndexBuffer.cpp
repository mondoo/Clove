#include "clvpch.hpp"
#include "DX11IndexBuffer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#include "Clove/Graphics/Renderer.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11IndexBuffer::DX11IndexBuffer(DX11IndexBuffer&& other) noexcept = default;

	DX11IndexBuffer& DX11IndexBuffer::operator=(DX11IndexBuffer&& other) noexcept = default;

	DX11IndexBuffer::~DX11IndexBuffer() = default;

	DX11IndexBuffer::DX11IndexBuffer(const std::vector<unsigned int>& indices){
		Renderer& renderer = Application::get().getRenderer();
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			DX11_INFO_PROVIDER(dxrenderer);

			count = static_cast<unsigned int>(indices.size());

			D3D11_BUFFER_DESC  ibd = { };
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0u;
			ibd.ByteWidth = static_cast<UINT>(indices.size() * sizeof(unsigned int));
			ibd.StructureByteStride = sizeof(unsigned int);

			D3D11_SUBRESOURCE_DATA isrd = { };
			isrd.pSysMem = indices.data();

			DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&ibd, &isrd, &indexBuffer));
		}
	}

	void DX11IndexBuffer::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			dxrenderer->getContext().IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
		}
	}

	void DX11IndexBuffer::unbind(){
	}

	unsigned int DX11IndexBuffer::getIndexCount() const{
		return count;
	}
}