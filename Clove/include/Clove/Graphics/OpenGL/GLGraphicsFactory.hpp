#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"

namespace clv::gfx::ogl{
	class GLGraphicsFactory : public GraphicsFactory{
		//FUNCTIONS
	public:
		GLGraphicsFactory();

		GLGraphicsFactory(const GLGraphicsFactory& other) = delete;
		GLGraphicsFactory(GLGraphicsFactory&& other) = delete;

		GLGraphicsFactory& operator=(const GLGraphicsFactory& other) = delete;
		GLGraphicsFactory& operator=(GLGraphicsFactory&& other) = delete;

		~GLGraphicsFactory();

		std::shared_ptr<CommandBuffer> createCommandBuffer() override;

		std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP) override;

		std::shared_ptr<PipelineObject> createPipelineObject() override;

		std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, std::string_view pathToShader) override;
		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, const char* bytes, const std::size_t size) override;

		std::shared_ptr<Surface> createSurface(void* windowData) override;
	};
}