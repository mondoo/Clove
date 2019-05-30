#include "clvpch.hpp"
#include "VertexBufferLayout.hpp"

namespace clv{
	namespace gfx{
		VertexBufferLayout::VertexBufferLayout() = default;

		VertexBufferLayout::VertexBufferLayout(VertexBufferLayout&& other) noexcept = default;

		VertexBufferLayout& VertexBufferLayout::operator=(VertexBufferLayout&& other) noexcept = default;

		VertexBufferLayout::~VertexBufferLayout() = default;
	}
}