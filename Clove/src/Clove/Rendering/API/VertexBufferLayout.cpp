#include "clvpch.hpp"
#include "VertexBufferLayout.hpp"

namespace clv{
	VertexBufferElement::VertexBufferElement()
		: normalised(GL_FALSE){
	}

	VertexBufferElement::VertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised)
		: type(inType)
		, count(inCount)
		, normalised(inNormalised){
	}

	VertexBufferLayout::VertexBufferLayout() = default;

	VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& other) = default;

	VertexBufferLayout::VertexBufferLayout(VertexBufferLayout&& other) = default;

	VertexBufferLayout::~VertexBufferLayout() = default;

	void VertexBufferLayout::empty(){
		elements.clear();
		stride = 0;
	}

	VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& other) = default;

	VertexBufferLayout& VertexBufferLayout::operator=(VertexBufferLayout&& other) = default;
}