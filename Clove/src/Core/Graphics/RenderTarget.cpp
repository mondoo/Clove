#include "RenderTarget.hpp"

#include "Core/Graphics/RenderAPI.hpp"
#include "Graphics/OpenGL-4/GL4RenderTarget.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11RenderTarget.hpp"
#endif

namespace clv::gfx{
	RenderTarget::RenderTarget() = default;

	RenderTarget::RenderTarget(RenderTarget&& other) noexcept = default;

	RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept = default;

	RenderTarget::~RenderTarget() = default;

	std::shared_ptr<RenderTarget> RenderTarget::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		switch(RenderDevice::getAPIType()){
			case API::OpenGL4:
				return std::make_shared<GL4RenderTarget>(colourTexture, depthStencilTexture);

			#if CLV_PLATFORM_WINDOWS
				case API::DirectX11:
					return std::make_shared<D3DRenderTarget>(colourTexture, depthStencilTexture);
			#endif

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", CLV_FUNCTION_NAME);
				return std::shared_ptr<RenderTarget>();
		}
	}
}