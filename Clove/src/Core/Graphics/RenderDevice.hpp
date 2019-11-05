#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderTarget;

	class RenderDevice{
		//FUNCTIONS
	public:
		RenderDevice() = default;
		RenderDevice(const RenderDevice& other) = delete;
		RenderDevice(RenderDevice&& other) noexcept = default;
		RenderDevice& operator=(const RenderDevice& other) = delete;
		RenderDevice& operator=(RenderDevice&& other) noexcept = default;
		virtual ~RenderDevice() = default;

		virtual void clear() = 0;
		virtual void drawIndexed(const uint32 count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;

		virtual void setDepthBuffer(bool enabled) = 0;

		virtual void setBlendState(bool enabled) = 0;

		virtual void setRenderTarget(RenderTarget& renderTarget) = 0;
		virtual void resetRenderTarget() = 0;

		virtual void setViewportSize(uint32 width, uint32 height) = 0;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		virtual void removeCurrentGeometryShader() = 0;
		virtual void removeTextureAtSlot(uint32 slot) = 0;
		//

		/*static API getAPIType();*/ //Hopefully not needed anymore
	};
}