#pragma once

namespace clv::gfx{
	class IndexBuffer;
	struct IndexBufferDescriptor;
	class ShaderBufferObject;
	struct ShaderBufferObjectDescriptor;
	class VertexBuffer;
	struct VertexBufferDescriptor;
	class Texture;
	struct TextureDescriptor;
}

namespace clv::gfx{
	class RenderFactory{
		//FUNCTIONS
	public:
		RenderFactory() = default;
		RenderFactory(const RenderFactory& other) = default;
		RenderFactory(RenderFactory&& other) = default;
		RenderFactory& operator=(const RenderFactory& other) = default;
		RenderFactory& operator=(RenderFactory&& other) = default;
		virtual ~RenderFactory() = default;

		virtual std::unique_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor) = 0;
		virtual std::unique_ptr<ShaderBufferObject> createShaderBufferObject(const ShaderBufferObjectDescriptor& descriptor) = 0;
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor) = 0;

		virtual std::unique_ptr<Texture> createTexture(const TextureDescriptor& descriptor) = 0;
	};
}