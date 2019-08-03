#include "clvpch.hpp"
#include "RenderTarget.hpp"

#include "Clove/Graphics/RenderAPI.hpp"
#include "Graphics/OpenGL-4/GL4RenderTarget.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11RenderTarget.hpp"
#endif

namespace clv::gfx{
	RenderTarget::RenderTarget() = default;

	RenderTarget::RenderTarget(RenderTarget&& other) noexcept = default;

	RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept = default;

	RenderTarget::~RenderTarget() = default;

	std::shared_ptr<RenderTarget> RenderTarget::createRenderTarget(Texture& writeTexture){
		switch(RenderAPI::getAPIType()){
			case API::OpenGL4:
				return std::make_shared<GL4RenderTarget>(writeTexture);

			#if CLV_PLATFORM_WINDOWS
				case API::DirectX11:
					return std::make_shared<DX11RenderTarget>(writeTexture);
			#endif

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", __func__);
				return std::shared_ptr<RenderTarget>();
		}
	}
}