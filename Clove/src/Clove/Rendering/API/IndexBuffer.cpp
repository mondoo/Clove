#include "clvpch.h"
#include "IndexBuffer.h"

#include "Clove/Rendering/Renderer.h"


//OpenGL specific for now

namespace clv{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int inCount)
		: count(inCount){
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer(){
		//Commented out because it gets called (because the temp object it copys/moves gets deleted)
		//GLCall(glDeleteBuffers(1, &rendererID));
	}

	void IndexBuffer::bind() const{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	}

	void IndexBuffer::unbind() const{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}