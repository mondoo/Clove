#pragma once

namespace clv::gfx{
	class IndexBuffer;
	class ShaderResource;
	class VertexBuffer;
	class Texture;
	class RenderTarget;
	class Shader;
	class Viewport;
}

namespace clv::gfx{
	class RenderDevice{
		//FUNCTIONS
	public:
		virtual void bindIndexBuffer(IndexBuffer& buffer) = 0;
		virtual void bindShaderBufferObject(ShaderResource& buffer, uint32 bindingPoint) = 0; //Temp uint32 - should be the enum
		virtual void bindVertexBuffer(VertexBuffer& buffer) = 0;

		virtual void bindTexture(Texture& texture, uint32 bindingPoint) = 0; //Temp uint32 - should be the enum

		virtual void bindShader(Shader& shader) = 0;

		virtual void setRenderTarget(RenderTarget& renderTarget) = 0;

		virtual void setViewport(const Viewport& viewport) = 0;

		virtual void clear() = 0;
		virtual void drawIndexed(const uint32 count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;
		virtual void setDepthBuffer(bool enabled) = 0;
		virtual void setBlendState(bool enabled) = 0;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		//virtual void removeCurrentGeometryShader() = 0;
		//virtual void removeTextureAtSlot(uint32 slot) = 0;
		//

		/*static API getAPIType();*/ //Hopefully not needed anymore
	};
}