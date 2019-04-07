#pragma once

//TODO: Remove (.inl needs the defs)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace clv{
	struct VertexBufferElement{
		//VARIABLES
	public:
		unsigned int type = 0;
		unsigned int count = 0;
		unsigned char normalised;

		//FUNCTIONS
	public:
		VertexBufferElement();
		VertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised);

		inline static unsigned int getSizeOfType(unsigned int type);
	};

	class VertexBufferLayout{
		//VARIABLES
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0;

		//FUNCTIONS
	public:
		template<typename T>
		void push(unsigned int count);

		template<>
		void push<float>(unsigned int count);

		template<>
		void push<unsigned int>(unsigned int count);

		template<>
		void push<unsigned char>(unsigned int count);

		inline const std::vector<VertexBufferElement>& getElements() const;
		inline unsigned int getStride() const;

		void empty();
	};
}

#include "VertexBufferLayout.inl"