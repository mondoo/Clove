#pragma once

namespace clv::gfx{
	class CommandBuffer;
	class Buffer;
	struct BufferDescriptor;
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
		virtual ~RenderFactory() = default;

		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(const std::shared_ptr<RenderTarget>& renderTarget) = 0;
		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(const std::shared_ptr<Surface>& surface) = 0;

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) = 0;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) = 0;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP) = 0;

		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) = 0;

		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) = 0;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) = 0;

		virtual std::shared_ptr<Surface> createSurface(void* windowData) = 0;
	};
}