#pragma once

#include "Clove/Graphics/RenderTarget.hpp"

namespace clv::gfx{
	class GL4RenderTarget : public RenderTarget{
		//VARIABLES
	private:
		uint32 renderID = 0;
		uint32 renderBufferID = 0;

		//FUNCTIONS
	public:
		GL4RenderTarget() = delete;
		GL4RenderTarget(const GL4RenderTarget& other) = delete;
		GL4RenderTarget(GL4RenderTarget&& other) noexcept;
		GL4RenderTarget& operator=(const GL4RenderTarget& other) = delete;
		GL4RenderTarget& operator=(GL4RenderTarget&& other) noexcept;
		virtual ~GL4RenderTarget();

		GL4RenderTarget(Texture& texture);

		const uint32 getRenderID() const;
	};
}
