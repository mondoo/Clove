#pragma once

#include "Clove/Graphics/Bindables/VertexBuffer.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv::gfx{
	class DX11VertexBuffer : public VertexBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

		//FUNCTIONS
	public:
		DX11VertexBuffer() = delete;
		DX11VertexBuffer(const DX11VertexBuffer& other) = delete;
		DX11VertexBuffer(DX11VertexBuffer&& other) noexcept;
		DX11VertexBuffer& operator=(const DX11VertexBuffer& other) = delete;
		DX11VertexBuffer& operator=(DX11VertexBuffer&& other) noexcept;
		~DX11VertexBuffer();

		DX11VertexBuffer(const std::vector<Vertex>& vertices);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;
	};
}