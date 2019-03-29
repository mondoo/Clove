#include "clvpch.h"
#include "VertexArray.h"
#include "Clove/Rendering/API/VertexBuffer.h"
#include "Clove/Rendering/API/VertexBufferLayout.h"
#include "Clove/Rendering/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace clv{
	VertexArray::VertexArray(){
		GLCall(glGenVertexArrays(1, &rendererID));
	}
	VertexArray::~VertexArray(){
		//GLCall(glDeleteVertexArrays(1, &rendererID));
	}

	void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
		bind();
		vb.bind();
		const auto& elements = layout.getElements();
		unsigned long long offset = 0; //void* expects 64 bits
		for(unsigned int i = 0; i < elements.size(); ++i){
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			//i is used here to communicate with the shader for what position the 'in' parameter is (see shader)
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

	void VertexArray::deleteArray(){
		GLCall(glDeleteVertexArrays(1, &rendererID));
	}
}