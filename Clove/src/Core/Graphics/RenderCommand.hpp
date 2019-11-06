#pragma once

namespace clv::gfx{
	class RenderDevice;
	class IndexBuffer;
	class VertexBuffer;
	class Texture;
	class RenderTarget;
	class Shader;
	class Viewport;
}

namespace clv::gfx{
	//NOTE: This will be used to create a multithreaded renderer. But for now it just wraps the API calls
	class RenderCommand{
		//VARIABLES
	private:
		static std::unique_ptr<RenderDevice> device;

		//FUNCTIONS
	public:
		static void bindIndexBuffer(IndexBuffer& buffer);
		static void bindVertexBuffer(VertexBuffer& buffer);
		
		static void bindShaderResource(ShaderResource& resource, uint32 bindingPoint); //Temp uint32 - should be the enum

		static void bindTexture(Texture& texture, uint32 bindingPoint); //Temp uint32 - should be the enum

		static void bindShader(Shader& shader);

		static void setRenderTarget(RenderTarget& renderTarget);

		static void setViewport(const Viewport& viewport);

		static void clear();
		static void drawIndexed(const uint32 count);

		static void setClearColour(const math::Vector4f& colour);
		static void setDepthBuffer(bool enabled);
		static void setBlendState(bool enabled);

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		//static void removeCurrentGeometryShader();
		//static void removeTextureAtSlot(uint32 slot);
		//

		static void initialiseRenderDevice();
	};
}
