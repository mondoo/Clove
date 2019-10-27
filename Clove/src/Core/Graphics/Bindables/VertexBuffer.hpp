#pragma once

#include "Core/Graphics/Bindable.hpp"

#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	class VertexBuffer : public Bindable{
		//VARIABLES
	protected:
		VertexBufferData bufferData;

		//FUNCTIONS
	public:
		VertexBuffer() = delete;
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		virtual ~VertexBuffer();

		VertexBuffer(const VertexBufferData& bufferData);
	};
}

