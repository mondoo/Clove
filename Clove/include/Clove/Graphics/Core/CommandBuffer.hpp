#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderTarget;
	class Buffer;
	class Texture;
	class PipelineObject;
	class Shader;
	class Surface;
	struct Viewport;
}

namespace clv::gfx{
	class CommandBuffer{
		//FUNCTIONS
	public:
		virtual ~CommandBuffer() = default;

		virtual void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) = 0;

		virtual void clearTarget() = 0;

		virtual void updateBufferData(const Buffer& buffer, const void* data) = 0;

		virtual void bindIndexBuffer(const Buffer& buffer) = 0;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32_t stride) = 0;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint) = 0;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) = 0;
		virtual void bindTexture(const Texture* texture, const uint32_t bindingPoint) = 0;

		virtual void setViewport(const Viewport& viewport) = 0;
		virtual void setDepthEnabled(bool enabled) = 0;

		virtual void drawIndexed(const uint32_t count) = 0;

		virtual void endEncoding() = 0;
	};
}