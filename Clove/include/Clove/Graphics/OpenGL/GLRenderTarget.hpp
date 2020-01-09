#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

namespace clv::gfx{
	class Texture;
};

namespace clv::gfx::ogl{
	class GLRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		uint32 frameBufferID = 0;
		uint32 renderBufferID = 0;

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
