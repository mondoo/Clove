#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv{
	namespace gfx{
		class GLVertexBuffer : public Bindable{
			//VARIABLES
		private:
			unsigned int rendererID = 0;

			//FUNCTIONS
		public:
			GLVertexBuffer() = delete;
			GLVertexBuffer(const GLVertexBuffer& other) = delete;
			GLVertexBuffer(GLVertexBuffer&& other) noexcept;
			GLVertexBuffer& operator=(const GLVertexBuffer& other) = delete;
			GLVertexBuffer& operator=(GLVertexBuffer&& other) noexcept;
			~GLVertexBuffer();

			GLVertexBuffer(const std::vector<float>& vertices);

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;
		};
	}
}