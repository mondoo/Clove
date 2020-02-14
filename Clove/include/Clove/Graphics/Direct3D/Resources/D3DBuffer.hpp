#pragma once

#include "Clove/Graphics/Core/Resources/Buffer.hpp"

#include <wrl.h>
#include <d3d11.h>

namespace clv::gfx::d3d{
	class D3DBuffer : public Buffer{
		//VARIABLES
	private:
		BufferDescriptor descriptor;

		Microsoft::WRL::ComPtr<ID3D11Buffer> d3dBuffer;
		
		//FUNCTIONS
	public:
		D3DBuffer() = delete;
		D3DBuffer(ID3D11Device& d3dDevice, const BufferDescriptor& descriptor, const void* data);

		D3DBuffer(const D3DBuffer& other) = delete;
		D3DBuffer(D3DBuffer&& other) noexcept;

		D3DBuffer& operator=(const D3DBuffer& other) = delete;
		D3DBuffer& operator=(D3DBuffer&& other) noexcept;

		virtual ~D3DBuffer();

		virtual const BufferDescriptor& getDescriptor() const override;

		virtual void updateData(const void* data) override;

		const Microsoft::WRL::ComPtr<ID3D11Buffer>& getD3DBuffer() const;

	private:
		UINT getD3DBufferType(BufferType cloveType);
		D3D11_USAGE getD3DBufferUsage(BufferUsage cloveUsage);
	};
}