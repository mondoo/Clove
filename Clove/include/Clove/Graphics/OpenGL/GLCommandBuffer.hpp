#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"

namespace clv::gfx::ogl {
	class GLRenderTarget;
	class GLSurface;
}

namespace clv::gfx::ogl {
	class GLCommandBuffer : public CommandBuffer {
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		std::vector<std::function<void()>> commands;

		std::shared_ptr<GLRenderTarget> glRenderTarget;

		//FUNCTIONS
	public:
		GLCommandBuffer(std::shared_ptr<GraphicsFactory> factory);

		GLCommandBuffer(const GLCommandBuffer& other) = delete;
		GLCommandBuffer(GLCommandBuffer&& other) noexcept;

		GLCommandBuffer& operator=(const GLCommandBuffer& other) = delete;
		GLCommandBuffer& operator=(GLCommandBuffer&& other) noexcept;

		~GLCommandBuffer();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) override;

		void clearTarget() override;

		void updateBufferData(Buffer& buffer, const void* data) override;

		void bindIndexBuffer(const Buffer& buffer) override;
		void bindVertexBuffer(const Buffer& buffer, const uint32_t stride) override;
		void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint) override;
		void bindPipelineObject(const PipelineObject& pipelineObject) override;
		void bindTexture(const Texture* texture, const uint32_t bindingPoint) override;

		void setViewport(const Viewport& viewport) override;
		void setDepthEnabled(bool enabled) override;

		void drawIndexed(const uint32_t count) override;

		void endEncoding() override;
	};
}
