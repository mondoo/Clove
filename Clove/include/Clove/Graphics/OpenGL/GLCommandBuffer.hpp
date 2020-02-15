#pragma once

#include "Clove/Graphics/Core/CommandBuffer.hpp"

namespace clv::gfx::ogl{
	class GLRenderTarget;
	class GLSurface;
}

namespace clv::gfx::ogl{
	class GLCommandBuffer : public CommandBuffer{
		//VARIABLES
	private:
		std::vector<std::function<void()>> commands;

		std::shared_ptr<GLRenderTarget> glRenderTarget;

		//FUNCTIONS
	public:
		GLCommandBuffer();

		GLCommandBuffer(const GLCommandBuffer& other) = delete;
		GLCommandBuffer(GLCommandBuffer&& other) noexcept;

		GLCommandBuffer& operator=(const GLCommandBuffer& other) = delete;
		GLCommandBuffer& operator=(GLCommandBuffer&& other) noexcept;

		virtual ~GLCommandBuffer();

		virtual void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) override;

		virtual void clearTarget() override;

		virtual void updateBufferData(const Buffer& buffer, const void* data) override;

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32_t stride) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture* texture, const uint32_t bindingPoint) override;

		virtual void setViewport(const Viewport& viewport) override;
		virtual void setDepthEnabled(bool enabled) override;

		virtual void drawIndexed(const uint32_t count) override;

		virtual void endEncoding() override;
	};
}