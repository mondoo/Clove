#pragma once

#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv::gfx{
	class DX11IndexBuffer : public IndexBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

		unsigned int count = 0;

		//FUNCTIONS
	public:
		DX11IndexBuffer() = delete;
		DX11IndexBuffer(const DX11IndexBuffer& other) = delete;
		DX11IndexBuffer(DX11IndexBuffer&& other) noexcept;
		DX11IndexBuffer& operator=(const DX11IndexBuffer& other) = delete;
		DX11IndexBuffer& operator=(DX11IndexBuffer&& other) noexcept;
		~DX11IndexBuffer();

		DX11IndexBuffer(const std::vector<unsigned int>& indices);

		virtual void bind() override;
		virtual void unbind() override;

		virtual unsigned int getIndexCount() const override;
		ID3D11Buffer* getBuffer() const;
	};
}
