#include "clvpch.hpp"
#include "VertexLayout.hpp"

namespace clv::gfx{
	VertexLayout::VertexLayout() = default;

	VertexLayout::VertexLayout(const VertexLayout& other) = default;

	VertexLayout::VertexLayout(VertexLayout&& other) noexcept = default;

	VertexLayout& VertexLayout::operator=(const VertexLayout& other) = default;

	VertexLayout& VertexLayout::operator=(VertexLayout&& other) noexcept = default;

	VertexLayout::~VertexLayout() = default;
}