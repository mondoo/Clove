#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

namespace clv::gfx{
	class Texture;
};

namespace clv::gfx::ogl{
	class GLRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };

		uint32_t frameBufferID = 0;
		uint32_t renderBufferID = 0;

		//FUNCTIONS
	public:
		GLRenderTarget(std::shared_ptr<GraphicsFactory> factory);
		GLRenderTarget(std::shared_ptr<GraphicsFactory> factory, Texture* colourTexture, Texture* depthStencilTexture);

		GLRenderTarget(const GLRenderTarget& other) = delete;
		GLRenderTarget(GLRenderTarget&& other) noexcept;

		GLRenderTarget& operator=(const GLRenderTarget& other) = delete;
		GLRenderTarget& operator=(GLRenderTarget&& other) noexcept;

		virtual ~GLRenderTarget();

		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual void clear() override;

		virtual void setClearColour(const mth::vec4f& colour) override;
		const mth::vec4f& getClearColour() const;

		uint32_t getGLFrameBufferID() const;
	};
}
