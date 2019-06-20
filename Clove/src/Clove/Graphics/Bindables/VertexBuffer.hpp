#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv::gfx{
	class VertexBuffer : public Bindable{
		//FUNCTIONS
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		virtual ~VertexBuffer();
	};
}

