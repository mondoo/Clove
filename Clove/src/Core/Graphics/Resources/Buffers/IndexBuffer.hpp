#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx{
	class IndexBuffer : public Buffer{
		//FUNCTIONS
	public:
		IndexBuffer() = default;
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept = default;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept = default;
		virtual ~IndexBuffer() = default;

		virtual uint32 getIndexCount() const = 0;
	};
}