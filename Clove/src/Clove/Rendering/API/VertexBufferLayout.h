#pragma once

//Temp - this is to get the GL_FLOAT etc (will need to abstract out or copy in somehow)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace clv{
	struct VertexBufferElement{
		//VARIABLES
	public:
		unsigned int type = 0;
		unsigned int count = 0;
		unsigned char normalised = GL_FALSE;

		//FUNCTIONS
	public:
		static unsigned int getSizeOfType(unsigned int type){
			switch(type){
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			default:
				CLV_CORE_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				return 0;
			}
		}
	};

	class CLV_API VertexBufferLayout{
		//VARIABLES
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0;

		//FUNCTIONS
	public:
		template<typename T>
		void push(unsigned int count){
			//static_assert(false);
			CLV_CORE_ASSERT(false, "Unsupported type for {0}", __FUNCTION__);
		}

		template <>
		void push<float>(unsigned int count){
			elements.push_back({ GL_FLOAT, count, GL_FALSE });
			stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
		}

		template <>
		void push<unsigned int>(unsigned int count){
			elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
		}

		template <>
		void push<unsigned char>(unsigned int count){
			elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& getElements() const{ return elements; }
		inline unsigned int getStride()const{ return stride; }
	};
}
