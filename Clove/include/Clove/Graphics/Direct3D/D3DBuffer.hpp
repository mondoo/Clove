#pragma once

#include "Clove/Graphics/Buffer.hpp"

#include <wrl.h>
#include <d3d11.h>

namespace clv::gfx::d3d{
	class D3DBuffer : public Buffer{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		BufferDescriptor descriptor;

		Microsoft::WRL::ComPtr<ID3D11Buffer> d3dBuffer;
		
		//FUNCTIONS
	public:
		D3DBuffer() = delete;
		D3DBuffer(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, const BufferDescriptor& descriptor, const void* data);

		D3DBuffer(const D3DBuffer& other) = delete;
		D3DBuffer(D3DBuffer&& other) noexcept;

		D3DBuffer& operator=(const D3DBuffer& other) = delete;
		D3DBuffer& operator=(D3DBuffer&& other) noexcept;

		~D3DBuffer();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		const BufferDescriptor& getDescriptor() const override;

		void updateData(const void* data) override;

		const Microsoft::WRL::ComPtr<ID3D11Buffer>& getD3DBuffer() const;
	};
}