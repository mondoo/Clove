#pragma once

#include "Clove/Graphics/Bindables/VertexBufferLayout.hpp"

namespace clv::gfx{
	class Renderer;

	struct GLVertexBufferElement{
		//VARIABLES
	public:
		unsigned int type = 0;
		unsigned int count = 0;
		unsigned char normalised;

		//FUNCTIONS
	public:
		GLVertexBufferElement();
		GLVertexBufferElement(const GLVertexBufferElement& other) = delete;
		GLVertexBufferElement(GLVertexBufferElement&& other) noexcept;
		GLVertexBufferElement& operator=(const GLVertexBufferElement& other) = delete;
		GLVertexBufferElement& operator=(GLVertexBufferElement&& other) noexcept;
		~GLVertexBufferElement();

		GLVertexBufferElement(unsigned int inType, unsigned int inCount, unsigned char inNormalised);

		static unsigned int getSizeOfType(unsigned int type);
	};

	class GLVertexBufferLayout : public VertexBufferLayout{
		//VARIABLES
	private:
		unsigned int arrayID = 0;

		unsigned int stride = 0;
		std::vector<GLVertexBufferElement> elements;

		//FUNCTIONS
	public:
		GLVertexBufferLayout();
		GLVertexBufferLayout(const GLVertexBufferLayout& other) = delete;
		GLVertexBufferLayout(GLVertexBufferLayout&& other) noexcept;
		GLVertexBufferLayout& operator=(const GLVertexBufferLayout& other) = delete;
		GLVertexBufferLayout& operator=(GLVertexBufferLayout&& other) noexcept;
		~GLVertexBufferLayout();

		virtual void pushElement(const std::string& elementName, BufferElementFormat elementFormat) override;
		virtual void createLayout(Bindable& bindable) override;
		virtual void resetLayout() override;

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;
	};
}