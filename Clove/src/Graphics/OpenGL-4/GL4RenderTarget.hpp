#pragma once

#include "Clove/Graphics/RenderTarget.hpp"

namespace clv::gfx{
	class GL4RenderTarget : public RenderTarget{
		//VARIABLES
	private:
		unsigned int renderID = 0;
		unsigned int renderBufferID = 0;

		//FUNCTIONS
	public:
		GL4RenderTarget() = delete;
		GL4RenderTarget(const GL4RenderTarget& other) = delete;
		GL4RenderTarget(GL4RenderTarget&& other) noexcept;
		GL4RenderTarget& operator=(const GL4RenderTarget& other) = delete;
		GL4RenderTarget& operator=(GL4RenderTarget&& other) noexcept;
		virtual ~GL4RenderTarget();

		GL4RenderTarget(Texture& texture);

		const unsigned int getRenderID() const;
	};
}
