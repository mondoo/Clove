#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Buffer;
	class Texture;
	class PipelineObject;
	class RenderTarget;
	class Shader;
	class Surface;
	struct Viewport;
}

namespace clv::gfx{
	class RenderDevice{
		//FUNCTIONS
	public:
		virtual ~RenderDevice() = default;

		virtual void bindIndexBuffer(const Buffer& buffer) = 0;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32 stride) = 0;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) = 0;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) = 0;
		virtual void bindTexture(const Texture* texture, const uint32 bindingPoint) = 0;
		virtual void bindShader(const Shader& shader) = 0;

		virtual void updateBufferData(Buffer& buffer, const void* data) = 0;

		virtual void makeSurfaceCurrent(const std::shared_ptr<Surface>& surface) = 0;

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		virtual void setRenderTarget(const RenderTarget* renderTarget) = 0;
		virtual void resetRenderTargetToDefault() = 0;
		//

		virtual void setViewport(const Viewport& viewport) = 0;

		virtual void clear() = 0;
		virtual void drawIndexed(const uint32 count) = 0;

		virtual void setClearColour(const mth::vec4f& colour) = 0;
		virtual void setDepthBuffer(bool enabled) = 0;
		virtual void setBlendState(bool enabled) = 0;
	};
}