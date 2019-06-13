#include "clvpch.hpp"
#include "GL4VertexBufferLayout.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/OpenGL-4/GL4Exception.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4VertexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4VertexBufferElement::GL4VertexBufferElement()
		: normalised(GL_FALSE){
	}

	GL4VertexBufferElement::GL4VertexBufferElement(GL4VertexBufferElement&& other) noexcept = default;

	GL4VertexBufferElement& GL4VertexBufferElement::operator=(GL4VertexBufferElement&& other) noexcept = default;

	GL4VertexBufferElement::~GL4VertexBufferElement() = default;

	GL4VertexBufferElement::GL4VertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised)
		: type(inType)
		, count(inCount)
		, normalised(inNormalised){
	}

	unsigned int GL4VertexBufferElement::getSizeOfType(unsigned int type){
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
		glGenVertexArrays(1, &arrayID);
	}

	GLVertexBufferLayout::GLVertexBufferLayout(GLVertexBufferLayout&& other) noexcept = default;

	GLVertexBufferLayout& GLVertexBufferLayout::operator=(GLVertexBufferLayout&& other) noexcept = default;

	GLVertexBufferLayout::~GLVertexBufferLayout(){
		glDeleteVertexArrays(1, &arrayID);
	}

	void GLVertexBufferLayout::pushElement(const std::string& elementName, BufferElementFormat elementFormat){
		switch(elementFormat){
			case BufferElementFormat::FLOAT_2:
				elements.emplace_back(GL_FLOAT, 2, GL_FALSE);
				stride += 2 * GL4VertexBufferElement::getSizeOfType(GL_FLOAT);
				break;

			case BufferElementFormat::FLOAT_3:
				elements.emplace_back(GL_FLOAT, 3, GL_FALSE);
				stride += 3 * GL4VertexBufferElement::getSizeOfType(GL_FLOAT);
				break;

			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				break;
		}
	}

	void GLVertexBufferLayout::createLayout(Bindable& bindable){
		if(GL4VertexBuffer* vb = dynamic_cast<GL4VertexBuffer*>(&bindable)){
			Renderer& renderer = Application::get().getRenderer();
			bind(renderer);
			vb->bind(renderer);

			unsigned long long offset = 0; //void* expects 64 bits
			for(unsigned int i = 0; i < elements.size(); ++i){
				const auto& element = elements[i];
				glEnableVertexAttribArray(i);
				//i is used here to communicate with the shader for what position the 'in' parameter is (see shader)
				glVertexAttribPointer(i, element.count, element.type, element.normalised, stride, reinterpret_cast<const void*>(offset));
				offset += element.count * GL4VertexBufferElement::getSizeOfType(element.type);
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
		glBindVertexArray(arrayID);
	}

	void GLVertexBufferLayout::unbind(){
		glBindVertexArray(0);
	}
}