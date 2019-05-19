#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
	class IndexBuffer : public Bindable{
		//FUNCTIONS
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		virtual ~IndexBuffer();

		virtual unsigned int getIndexCount() const = 0;
	};
}