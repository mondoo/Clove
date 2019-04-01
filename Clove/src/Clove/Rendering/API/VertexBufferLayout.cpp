#include "clvpch.h"
#include "VertexBufferLayout.h"

namespace clv{
	VertexBufferElement::VertexBufferElement()
		: normalised(GL_FALSE){
	}

	VertexBufferElement::VertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised)
		: type(inType)
		, count(inCount)
		, normalised(inNormalised){
	}
}