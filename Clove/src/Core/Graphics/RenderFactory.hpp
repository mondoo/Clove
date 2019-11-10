#pragma once

namespace clv::gfx{
	class Buffer;
	struct BufferDescriptor;
	class ShaderResource;
	struct ShaderResourceDescriptor;
	class Texture;
	class PipelineObject;
	struct TextureDescriptor;
	class RenderTarget;
	class Shader;
	struct ShaderDescriptor;
	class Surface;
	class VertexLayout;
}

namespace clv::gfx{
	class RenderFactory{
		//FUNCTIONS
	public:
		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, void* data) = 0;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) = 0;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, void* data, int32 BPP) = 0;
		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) = 0;
		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) = 0;
		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) = 0;
		virtual std::shared_ptr<Surface> createSurface() = 0;
	};
}