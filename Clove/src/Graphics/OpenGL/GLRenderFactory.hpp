#pragma once

#include "Core/Graphics/RenderFactory.hpp"

namespace clv::gfx::ogl{
	class GLRenderFactory : public RenderFactory{
		//FUNCTIONS
	public:
		GLRenderFactory();
		GLRenderFactory(const GLRenderFactory& other) = delete;
		GLRenderFactory(GLRenderFactory&& other) = delete;
		GLRenderFactory& operator=(const GLRenderFactory& other) = delete;
		GLRenderFactory& operator=(GLRenderFactory&& other) = delete;
		~GLRenderFactory();

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP) override;
		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) override;
		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;
		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;
		virtual std::shared_ptr<Surface> createSurface() override;
	};
}