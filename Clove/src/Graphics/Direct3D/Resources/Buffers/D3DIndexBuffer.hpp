#pragma once

#include "Core/Graphics/Resources/Buffers/IndexBuffer.hpp"

#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11Device;

namespace clv::gfx::d3d::_11{
	class D3DIndexBuffer : public IndexBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

		uint32 count = 0;

		//FUNCTIONS
	public:
		D3DIndexBuffer() = delete;
		D3DIndexBuffer(ID3D11Device& d3dDevice, const IndexBufferDescriptor& descriptor, void* indices);
		D3DIndexBuffer(const D3DIndexBuffer& other) = delete;
		D3DIndexBuffer(D3DIndexBuffer&& other) noexcept;
		D3DIndexBuffer& operator=(const D3DIndexBuffer& other) = delete;
		D3DIndexBuffer& operator=(D3DIndexBuffer&& other) noexcept;
		~D3DIndexBuffer();

		virtual uint32 getIndexCount() const override;
		Microsoft::WRL::ComPtr<ID3D11Buffer> getD3DBuffer() const;
	};
}
