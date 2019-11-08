#include "D3DIndexBuffer.hpp"

#include "Graphics/Direct3D/D3DException.hpp"
#include "Graphics/Direct3D/D3DRenderFactory.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d::_11{
	D3DIndexBuffer::D3DIndexBuffer(ID3D11Device& d3dDevice, const IndexBufferDescriptor& descriptor, void* indices){
		count = descriptor.indexCount;
		
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth			= static_cast<UINT>(descriptor.bufferSize);
		bufferDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags		= 0u;
		bufferDesc.MiscFlags			= 0u;
		bufferDesc.StructureByteStride	= descriptor.elementsize;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem			= indices;
		initData.SysMemPitch		= 0u;
		initData.SysMemSlicePitch	= 0u;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateBuffer(&bufferDesc, &initData, &indexBuffer));
	}

	D3DIndexBuffer::D3DIndexBuffer(D3DIndexBuffer&& other) noexcept = default;

	D3DIndexBuffer& D3DIndexBuffer::operator=(D3DIndexBuffer&& other) noexcept = default;

	D3DIndexBuffer::~D3DIndexBuffer() = default;

	uint32 D3DIndexBuffer::getIndexCount() const{
		return count;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> D3DIndexBuffer::getD3DBuffer() const{
		return indexBuffer;
	}
}