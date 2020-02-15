#pragma once

#include "Clove/Graphics/Core/GraphicsFactory.hpp"

namespace clv::gfx::ogl{
	class GLGraphicsFactory : public GraphicsFactory{
		//FUNCTIONS
	public:
		GLGraphicsFactory();

		GLGraphicsFactory(const GLGraphicsFactory& other) = delete;
		GLGraphicsFactory(GLGraphicsFactory&& other) = delete;

		GLGraphicsFactory& operator=(const GLGraphicsFactory& other) = delete;
		GLGraphicsFactory& operator=(GLGraphicsFactory&& other) = delete;

		virtual ~GLGraphicsFactory();

		virtual std::shared_ptr<CommandBuffer> createCommandBuffer() override;

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP) override;

		virtual std::shared_ptr<PipelineObject> createPipelineObject() override;

		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor, std::string_view pathToShader) override;

		virtual std::shared_ptr<Surface> createSurface(void* windowData) override;
	};
}