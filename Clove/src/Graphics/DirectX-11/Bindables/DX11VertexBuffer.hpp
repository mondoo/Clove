#pragma once

#include "Core/Graphics/Bindables/VertexBuffer.hpp"

#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace clv::gfx{
	class Shader;
	
	class DX11VertexBuffer : public VertexBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

		//could have a thing on this to rfresh the layout
		//or rebind the shader. Assert in the bindy boys if that hasn't happened yet
		//I think the complicated because of the input layout

		//FUNCTIONS
	public:
		DX11VertexBuffer() = delete;
		DX11VertexBuffer(const DX11VertexBuffer& other) = delete;
		DX11VertexBuffer(DX11VertexBuffer&& other) noexcept;
		DX11VertexBuffer& operator=(const DX11VertexBuffer& other) = delete;
		DX11VertexBuffer& operator=(DX11VertexBuffer&& other) noexcept;
		~DX11VertexBuffer();

		DX11VertexBuffer(const VertexBufferData& bufferData, Shader& shader);

		virtual void bind() override;
	};
}