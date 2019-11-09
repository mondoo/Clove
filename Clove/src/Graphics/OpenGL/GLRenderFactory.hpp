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

		virtual std::shared_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor, void* indices) override;
		virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor) override;

		virtual std::shared_ptr<ShaderResource> createShaderResource(const ShaderResourceDescriptor& descriptor) override;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor) override;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;

		virtual std::shared_ptr<Surface> createSurface() override;
	};
}