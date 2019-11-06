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
	class Surface;
	class Viewport;
}

namespace clv::gfx{
	class RenderFactory{
		//FUNCTIONS
	public:
		virtual std::unique_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor) = 0;
		virtual std::unique_ptr<ShaderBufferObject> createShaderBufferObject(const ShaderBufferObjectDescriptor& descriptor) = 0;
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor) = 0;

		virtual std::unique_ptr<Texture> createTexture(const TextureDescriptor& descriptor) = 0;

		virtual std::unique_ptr<Surface> createSurface() = 0;

		virtual std::unique_ptr<Viewport> createViewport() = 0;
	};
}