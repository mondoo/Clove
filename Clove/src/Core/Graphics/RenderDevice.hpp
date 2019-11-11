#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Buffer;
	class Texture;
	class PipelineObject;
	class RenderTarget;
	class Shader;
	class Surface;
	class Viewport;
}

namespace clv::gfx{
	class RenderDevice{
		//FUNCTIONS
	public:
		virtual void bindIndexBuffer(const Buffer& buffer) = 0;
		virtual void bindVertexBuffer(const Buffer& buffer) = 0;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) = 0;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) = 0;
		virtual void bindTexture(const Texture& texture, const uint32 bindingPoint) = 0;
		virtual void bindShader(const Shader& shader) = 0;

		virtual void updateBufferData(Buffer& buffer, const void* data) = 0;

		virtual void makeSurfaceCurrent(Surface& surface) = 0;

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		virtual void setRenderTarget(RenderTarget& renderTarget) = 0;
		virtual void resetRenderTargetToDefault() = 0;
		//

		virtual void setViewport(const Viewport& viewport) = 0;

		virtual void clear() = 0;
		virtual void drawIndexed(const uint32 count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;
		virtual void setDepthBuffer(bool enabled) = 0;
		virtual void setBlendState(bool enabled) = 0;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		virtual void removeCurrentGeometryShader() = 0;
		virtual void removeTextureAtSlot(uint32 slot) = 0;
		//
	};
}