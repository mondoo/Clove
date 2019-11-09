#pragma once

#include "Core/Graphics/RenderTarget.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GL4RenderTarget : public RenderTarget{
		//VARIABLES
	private:
		GLuint frameBufferID = 0;
		GLuint renderBufferID = 0;

		//FUNCTIONS
	public:
		GL4RenderTarget() = delete;
		GL4RenderTarget(Texture* colourTexture, Texture* depthStencilTexture);
		GL4RenderTarget(const GL4RenderTarget& other) = delete;
		GL4RenderTarget(GL4RenderTarget&& other) noexcept;
		GL4RenderTarget& operator=(const GL4RenderTarget& other) = delete;
		GL4RenderTarget& operator=(GL4RenderTarget&& other) noexcept;
		virtual ~GL4RenderTarget();

		const uint32 getGLFrameBufferID() const;
	};
}
