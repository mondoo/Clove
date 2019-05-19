#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
	enum class BufferElementFormat{
		FLOAT_2,
		FLOAT_3
	};

	class VertexBufferLayout : public Bindable{
		//FUNCTIONS
	public:
		VertexBufferLayout();
		VertexBufferLayout(const VertexBufferLayout& other) = delete;
		VertexBufferLayout(VertexBufferLayout&& other) noexcept;
		VertexBufferLayout& operator=(const VertexBufferLayout& other) = delete;
		VertexBufferLayout& operator=(VertexBufferLayout&& other) noexcept;
		~VertexBufferLayout();

		virtual void pushElement(const std::string& elementName, BufferElementFormat elementFormat) = 0;
		virtual void createLayout(Bindable& bindable, Renderer& renderer) = 0;
		virtual void resetLayout() = 0;
	};
}