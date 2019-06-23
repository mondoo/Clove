#include "clvpch.hpp"
#include "GL4VertexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4VertexBuffer::GL4VertexBuffer(GL4VertexBuffer&& other) noexcept = default;

	GL4VertexBuffer& GL4VertexBuffer::operator=(GL4VertexBuffer&& other) noexcept = default;

	GL4VertexBuffer::~GL4VertexBuffer(){
		glDeleteVertexArrays(1, &arrayID);
		glDeleteBuffers(1, &bufferID);
	}

	GL4VertexBuffer::GL4VertexBuffer(const VertexBufferData& bufferData, Shader& shader)
		: bufferData(bufferData){
		const unsigned int size = static_cast<unsigned int>(this->bufferData.sizeBytes());
		const void* data = this->bufferData.data();

		glGenVertexArrays(1, &arrayID);
		glBindVertexArray(arrayID);
		
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		unsigned long long offset = 0; //void* expects 64 bits
		for(unsigned int i = 0; i < this->bufferData.getLayout().count(); ++i){
			const auto& element = this->bufferData.getLayout().resolve(i);

			unsigned int elemType = 0;
			unsigned char elemNormalised = GL_FALSE;

			switch(element.getType()){
				case VertexElementType::position2D:
					elemType = GL_FLOAT;
					elemNormalised = GL_FALSE;
					break;
				case VertexElementType::position3D:
					elemType = GL_FLOAT;
					elemNormalised = GL_FALSE;
					break;
				case VertexElementType::texture2D:
					elemType = GL_FLOAT;
					elemNormalised = GL_FALSE;
					break;
				case VertexElementType::normal:
					elemType = GL_FLOAT;
					elemNormalised = GL_FALSE;
					break;
				default:
					CLV_ASSERT(false, "Invalid element type");
					continue;
			}

			glEnableVertexAttribArray(i);
			//i is used here to communicate with the shader for what position the 'in' parameter is (see shader)
			glVertexAttribPointer(i, element.getCount(), elemType, elemNormalised, this->bufferData.getLayout().size(), reinterpret_cast<const void*>(offset));
			offset += /*element.count * */VertexElement::sizeOf(element.getType());
		}
	}

	//GL4VertexBuffer::GL4VertexBuffer(const std::vector<Vertex>& vertices){
	//	const unsigned int size = static_cast<unsigned int>(sizeof(vertices) * sizeof(Vertex));
	//	const void* data = vertices.data();

	//	glGenBuffers(1, &bufferID);
	//	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//}

	void GL4VertexBuffer::bind(Renderer& renderer){
		glBindVertexArray(arrayID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	void GL4VertexBuffer::unbind(){
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}