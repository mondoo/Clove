#pragma once

//Temp - this is to get the GL_FLOAT etc (will need to abstract out or copy in somehow)
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

//TODO: abstract properly - taken from the gl headers
#define CLV_FALSE 0
#define CLV_TRUE 1

#define CLV_FLOAT 0x1406
#define CLV_UNSIGNED_INT 0x1405
#define CLV_UNSIGNED_BYTE 0x1401
//

namespace clv{
	struct VertexBufferElement{
		//VARIABLES
	public:
		unsigned int type = 0;
		unsigned int count = 0;
		unsigned char normalised = CLV_FALSE;

		//FUNCTIONS
	public:
		static unsigned int getSizeOfType(unsigned int type){
			switch(type){
			case CLV_FLOAT:
				return 4;
			case CLV_UNSIGNED_INT:
				return 4;
			case CLV_UNSIGNED_BYTE:
				return 1;
			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
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
			CLV_ASSERT(false, "Unsupported type for {0}", __FUNCTION__);
		}

		template <>
		void push<float>(unsigned int count){
			elements.push_back({ CLV_FLOAT, count, CLV_FALSE });
			stride += count * VertexBufferElement::getSizeOfType(CLV_FLOAT);
		}

		template <>
		void push<unsigned int>(unsigned int count){
			elements.push_back({ CLV_UNSIGNED_INT, count, CLV_FALSE });
			stride += count * VertexBufferElement::getSizeOfType(CLV_UNSIGNED_INT);
		}

		template <>
		void push<unsigned char>(unsigned int count){
			elements.push_back({ CLV_UNSIGNED_BYTE, count, CLV_TRUE });
			stride += count * VertexBufferElement::getSizeOfType(CLV_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& getElements() const{ return elements; }
		inline unsigned int getStride()const{ return stride; }
	};
}
