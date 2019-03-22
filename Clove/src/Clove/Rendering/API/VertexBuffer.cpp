#include "clvpch.h"
#include "VertexBuffer.h"

#include "Clove/Rendering/Renderer.h"


//OpenGL specific for now

namespace clv{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size){
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer(){
		//Commented out because it gets called (because the temp object it copys/moves gets deleted)
		//GLCall(glDeleteBuffers(1, &rendererID));
	}

	void VertexBuffer::bind() const{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	}

	void VertexBuffer::unbind() const{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}