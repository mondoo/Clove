#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	class Texture;
};

namespace clv::gfx::ogl{
	class GLRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		GLuint frameBufferID = 0;
		GLuint renderBufferID = 0;

		//FUNCTIONS
	public:
		GLRenderTarget();
		GLRenderTarget(Texture* colourTexture, Texture* depthStencilTexture);

		GLRenderTarget(const GLRenderTarget& other) = delete;
		GLRenderTarget(GLRenderTarget&& other) noexcept;

		GLRenderTarget& operator=(const GLRenderTarget& other) = delete;
		GLRenderTarget& operator=(GLRenderTarget&& other) noexcept;

		virtual ~GLRenderTarget();

		const uint32 getGLFrameBufferID() const;
	};
}
