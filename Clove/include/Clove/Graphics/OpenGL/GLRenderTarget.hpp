#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

namespace clv::gfx{
	class Texture;
};

namespace clv::gfx::ogl{
	class GLRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		uint32_t lockCount = 0;
		mth::vec4f clearColour;
		bool canClear = true;

		uint32_t frameBufferID = 0;
		uint32_t renderBufferID = 0;

		//FUNCTIONS
	public:
		GLRenderTarget();
		GLRenderTarget(Texture* colourTexture, Texture* depthStencilTexture);

		GLRenderTarget(const GLRenderTarget& other) = delete;
		GLRenderTarget(GLRenderTarget&& other) noexcept;

		GLRenderTarget& operator=(const GLRenderTarget& other) = delete;
		GLRenderTarget& operator=(GLRenderTarget&& other) noexcept;

		virtual ~GLRenderTarget();

		void lock();
		void unlock();

		virtual void setClearColour(const mth::vec4f& colour) override;
		void clear();

		uint32_t getGLFrameBufferID() const;
	};
}
