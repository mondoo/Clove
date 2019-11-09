#include "GLRenderFactory.hpp"

namespace clv::gfx::ogl{
	GLRenderFactory::GLRenderFactory() = default;

	GLRenderFactory::~GLRenderFactory() = default;

	std::shared_ptr<IndexBuffer> GLRenderFactory::createIndexBuffer(const IndexBufferDescriptor& descriptor, void* indices){
		//TODO
	}

	std::shared_ptr<VertexBuffer> GLRenderFactory::createVertexBuffer(const VertexBufferDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<ShaderResource> GLRenderFactory::createShaderResource(const ShaderResourceDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Texture> GLRenderFactory::createTexture(const TextureDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Shader> GLRenderFactory::createShader(const ShaderDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Surface> GLRenderFactory::createSurface(){
		//TODO
	}
}