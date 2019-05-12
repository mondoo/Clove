#include "clvpch.hpp"
#include "DX11IndexBuffer.hpp"

#include "GraphicsAPI/DX11/DX11Exception.hpp"
#include "GraphicsAPI/DX11/DX11Renderer.hpp"

#include <d3d11.h>

namespace clv{
	namespace gfx{
		DX11IndexBuffer::DX11IndexBuffer(DX11IndexBuffer && other) noexcept = default;

		DX11IndexBuffer& DX11IndexBuffer::operator=(DX11IndexBuffer&& other) noexcept = default;

		DX11IndexBuffer::~DX11IndexBuffer() = default;

		DX11IndexBuffer::DX11IndexBuffer(const std::vector<unsigned short>& indices, Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				DX11_INFO_PROVIDER(dxrenderer);

				D3D11_BUFFER_DESC  ibd = { };
				ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				ibd.Usage = D3D11_USAGE_DEFAULT;
				ibd.CPUAccessFlags = 0;
				ibd.MiscFlags = 0u;
				ibd.ByteWidth = indices.size() * sizeof(unsigned short);
				ibd.StructureByteStride = sizeof(unsigned short);

				D3D11_SUBRESOURCE_DATA isrd = { };
				isrd.pSysMem = indices.data();

				DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&ibd, &isrd, &indexBuffer));
			}
		}

		void DX11IndexBuffer::bind(Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				dxrenderer->getContext().IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
			}
		}

		void DX11IndexBuffer::unbind(){
		}
	}
}