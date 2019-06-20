#pragma once

#include "Clove/Graphics/Bindables/VertexBuffer.hpp"

namespace clv::gfx{
	class GL4VertexBuffer : public VertexBuffer{
		//VARIABLES
	private:
		unsigned int bufferID = 0;

		//FUNCTIONS
	public:
		GL4VertexBuffer() = delete;
		GL4VertexBuffer(const GL4VertexBuffer& other) = delete;
		GL4VertexBuffer(GL4VertexBuffer&& other) noexcept;
		GL4VertexBuffer& operator=(const GL4VertexBuffer& other) = delete;
		GL4VertexBuffer& operator=(GL4VertexBuffer&& other) noexcept;
		~GL4VertexBuffer();

		/*GL4VertexBuffer(const std::vector<Vertex>& vertices);*/

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;
	};
}