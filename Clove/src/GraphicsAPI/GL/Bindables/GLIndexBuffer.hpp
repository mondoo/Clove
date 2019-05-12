#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv{
	namespace gfx{
		class Renderer;

		class GLIndexBuffer : public Bindable{
			//VARIABLES
		private:
			unsigned int rendererID = 0;
			unsigned int count = 0;

			//FUNCTIONS
		public:
			GLIndexBuffer() = delete;
			GLIndexBuffer(const GLIndexBuffer& other) = delete;
			GLIndexBuffer(GLIndexBuffer&& other) noexcept;
			GLIndexBuffer& operator=(const GLIndexBuffer& other) = delete;
			GLIndexBuffer& operator=(GLIndexBuffer&& other) noexcept;
			~GLIndexBuffer();

			GLIndexBuffer(const std::vector<unsigned int>& indices);

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;
		};
	}
}
