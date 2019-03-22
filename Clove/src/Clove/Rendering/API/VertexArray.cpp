#include "clvpch.h"
#include "VertexArray.h"
#include "Clove/Rendering/API/VertexBuffer.h"
#include "Clove/Rendering/API/VertexBufferLayout.h"
#include "Clove/Rendering/Renderer.h"

namespace clv{
	VertexArray::VertexArray(){
		GLCall(glGenVertexArrays(1, &rendererID));
	}
	VertexArray::~VertexArray(){
		GLCall(glDeleteVertexArrays(1, &rendererID));
	}

	void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
		bind();
		vb.bind();
		const auto& elements = layout.getElements();
		unsigned int offset = 0;
		for(unsigned int i = 0; i < elements.size(); ++i){
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), reinterpret_cast<const void*>(offset)));
			offset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
	}

	void VertexArray::bind() const{
		GLCall(glBindVertexArray(rendererID));
	}

	void VertexArray::unbind() const{
		GLCall(glBindVertexArray(0));
	}
}