#pragma once

#include "Core/Graphics/RenderAPI.hpp"

namespace clv::gfx{
	class Context;

	class GL4RenderAPI : public RenderDevice{
		//FUNCTIONS
	public:
		GL4RenderAPI() = delete;
		GL4RenderAPI(const GL4RenderAPI& other) = delete;
		GL4RenderAPI(GL4RenderAPI&& other) noexcept = delete;
		GL4RenderAPI& operator=(const GL4RenderAPI& other) = delete;
		GL4RenderAPI& operator=(GL4RenderAPI&& other) noexcept = delete;
		~GL4RenderAPI();

		GL4RenderAPI(const Context& context);

		virtual void clear() override;
		virtual void drawIndexed(const uint32 count) override;

		virtual void setClearColour(const math::Vector4f& colour) override;

		virtual void setDepthBuffer(bool enabled) override;

		virtual void setBlendState(bool enabled) override;

		virtual void setRenderTarget(RenderTarget& renderTarget) override;
		virtual void resetRenderTarget() override;

		virtual void setViewportSize(uint32 width, uint32 height) override;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		virtual void removeCurrentGeometryShader() override;
		virtual void removeTextureAtSlot(uint32 slot) override;
		//
	};
}