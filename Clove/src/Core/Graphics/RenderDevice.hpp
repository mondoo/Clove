#pragma once

namespace clv::gfx{
	class IndexBuffer;
	class VertexBuffer;
	class ShaderResource;
	class Texture;
	class RenderTarget;
	class Shader;
	class Surface;
	class Viewport;
}

namespace clv::gfx{
	class RenderDevice{
		//FUNCTIONS
	public:
		virtual void bindIndexBuffer(IndexBuffer& buffer) = 0;
		virtual void bindVertexBuffer(VertexBuffer& buffer) = 0;
		
		virtual void bindShaderResource(ShaderResource& resource) = 0;

		virtual void bindTexture(Texture& texture) = 0;

		virtual void bindShader(Shader& shader) = 0;

		virtual void makeSurfaceCurrent(const Surface& surface) = 0;

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		virtual void setRenderTarget(RenderTarget& renderTarget) = 0;
		virtual void resetRenderTargetToDefault() = 0;
		//

		virtual void setViewport(const Viewport& viewport) = 0;

		virtual void clear() = 0;
		virtual void drawIndexed(const uint32 count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;
		virtual void setDepthBuffer(bool enabled) = 0;
		virtual void setBlendState(bool enabled) = 0;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		virtual void removeCurrentGeometryShader() = 0;
		virtual void removeTextureAtSlot(uint32 slot) = 0;
		//

		/*static API getAPIType();*/ //Hopefully not needed anymore
	};
}