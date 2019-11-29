#include "Clove/Graphics/Direct3D/Resources/D3DBuffer.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderFactory.hpp"
#if CLV_DEBUG
#include "Clove/Graphics/Direct3D/D3DRenderDevice.hpp"
#endif

namespace clv::gfx::d3d{
	D3DBuffer::D3DBuffer(ID3D11Device& d3dDevice, const BufferDescriptor& descriptor, const void* data)
		: descriptor(descriptor){

		const BufferUsage bufferUsage = descriptor.bufferUsage;
		const BufferType bufferType = descriptor.bufferType;

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth			= static_cast<UINT>(descriptor.bufferSize);
		bufferDesc.Usage				= getD3DBufferUsage(bufferUsage);
		bufferDesc.BindFlags			= getD3DBufferType(bufferType);
		bufferDesc.CPUAccessFlags		= (bufferUsage == BufferUsage::Dynamic) ? D3D11_CPU_ACCESS_WRITE : 0u;
		bufferDesc.MiscFlags			= 0u;
		bufferDesc.StructureByteStride	= static_cast<UINT>(descriptor.elementSize);

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem			= data;
		initData.SysMemPitch		= 0u;
		initData.SysMemSlicePitch	= 0u;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateBuffer(&bufferDesc, &initData, &d3dBuffer));
	}

	D3DBuffer::D3DBuffer(D3DBuffer&& other) noexcept = default;

	D3DBuffer& D3DBuffer::operator=(D3DBuffer&& other) noexcept = default;

	D3DBuffer::~D3DBuffer() = default;

	Microsoft::WRL::ComPtr<ID3D11Buffer> D3DBuffer::getD3DBuffer() const{
		return d3dBuffer;
	}

	const BufferDescriptor& D3DBuffer::getDescriptor() const{
		return descriptor;
	}

	UINT D3DBuffer::getD3DBufferType(BufferType cloveType){
		switch(cloveType){
			case BufferType::IndexBuffer:
				return D3D11_BIND_INDEX_BUFFER;
			case BufferType::VertexBuffer:
				return D3D11_BIND_VERTEX_BUFFER;
			case BufferType::ShaderResourceBuffer:
				return D3D11_BIND_CONSTANT_BUFFER;
			default:
				CLV_ASSERT(false, "Unknown buffer type in {0}", CLV_FUNCTION_NAME);
				return 0u;
		}
	}

	D3D11_USAGE D3DBuffer::getD3DBufferUsage(BufferUsage cloveUsage){
		switch(cloveUsage){
			case BufferUsage::Default:
				return D3D11_USAGE_DEFAULT;
			case BufferUsage::Dynamic:
				return D3D11_USAGE_DYNAMIC;
			default:
				CLV_ASSERT(false, "Unknown buffer usage in {0}", CLV_FUNCTION_NAME);
				return D3D11_USAGE_DEFAULT;
		}
	}
}