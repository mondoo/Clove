#include "clvpch.hpp"
#include "GLVertexBufferLayout.hpp"

#include "Graphics/OpenGL/GLException.hpp"
#include "Graphics/OpenGL/Bindables/GLVertexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GLVertexBufferElement::GLVertexBufferElement()
		: normalised(GL_FALSE){
	}

	GLVertexBufferElement::GLVertexBufferElement(GLVertexBufferElement&& other) noexcept = default;

	GLVertexBufferElement& GLVertexBufferElement::operator=(GLVertexBufferElement&& other) noexcept = default;

	GLVertexBufferElement::~GLVertexBufferElement() = default;

	GLVertexBufferElement::GLVertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised)
		: type(inType)
		, count(inCount)
		, normalised(inNormalised){
	}

	unsigned int GLVertexBufferElement::getSizeOfType(unsigned int type){
		switch(type){
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				return 0;
		}
	}

	GLVertexBufferLayout::GLVertexBufferLayout(){
		GLCall(glGenVertexArrays(1, &arrayID));
	}

	GLVertexBufferLayout::GLVertexBufferLayout(GLVertexBufferLayout&& other) noexcept = default;

	GLVertexBufferLayout& GLVertexBufferLayout::operator=(GLVertexBufferLayout&& other) noexcept = default;

	GLVertexBufferLayout::~GLVertexBufferLayout(){
		GLCall(glDeleteVertexArrays(1, &arrayID));
	}

	void GLVertexBufferLayout::pushElement(const std::string& elementName, BufferElementFormat elementFormat){
		switch(elementFormat){
			case BufferElementFormat::FLOAT_2:
				elements.emplace_back(GL_FLOAT, 2, GL_FALSE);
				stride += 2 * GLVertexBufferElement::getSizeOfType(GL_FLOAT);
				break;

			case BufferElementFormat::FLOAT_3:
				elements.emplace_back(GL_FLOAT, 3, GL_FALSE);
				stride += 3 * GLVertexBufferElement::getSizeOfType(GL_FLOAT);
				break;

			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				break;
		}
	}

	void GLVertexBufferLayout::createLayout(Bindable& bindable, Renderer& renderer){
		if(GLVertexBuffer* vb = dynamic_cast<GLVertexBuffer*>(&bindable)){
			bind(renderer);
			vb->bind(renderer);

			unsigned long long offset = 0; //void* expects 64 bits
			for(unsigned int i = 0; i < elements.size(); ++i){
				const auto& element = elements[i];
				GLCall(glEnableVertexAttribArray(i));
				//i is used here to communicate with the shader for what position the 'in' parameter is (see shader)
				GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, stride, reinterpret_cast<const void*>(offset)));
				offset += element.count * GLVertexBufferElement::getSizeOfType(element.type);
			}
		} else{
			//CLV_LOG_ERROR(__FUNCTION__ " Wrong typed passed, can't create layout");
			//TODO: Platform thing
		}
	}

	void GLVertexBufferLayout::resetLayout(){
		elements.clear();
	}

	void GLVertexBufferLayout::bind(Renderer& renderer){
		GLCall(glBindVertexArray(arrayID));
	}

	void GLVertexBufferLayout::unbind(){
		GLCall(glBindVertexArray(0));
	}
}