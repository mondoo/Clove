#pragma once

namespace clv::gfx{
	class Texture;

	class RenderTarget{
		//FUNCTIONS
	public:
		RenderTarget();
		RenderTarget(const RenderTarget& other) = delete;
		RenderTarget(RenderTarget&& other) noexcept;
		RenderTarget& operator=(const RenderTarget& other) = delete;
		RenderTarget& operator=(RenderTarget&& other) noexcept;
		virtual ~RenderTarget();

		static std::shared_ptr<RenderTarget> createRenderTarget(Texture& writeTexture);
	};
}
