#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/RenderDevice.hpp"
#include "Core/Graphics/RenderFactory.hpp"

namespace clv::gfx{
	//NOTE: This will be used to create a multithreaded renderer. But for now it just wraps the API calls
	class RenderCommand{
		//VARIABLES
	private:
		static std::unique_ptr<RenderDevice> device;
		static std::unique_ptr<RenderFactory> factory;

		//FUNCTIONS
	public:
		static void bindIndexBuffer(IndexBuffer& buffer);
		static void bindVertexBuffer(VertexBuffer& buffer);
		
		static void bindShaderResource(ShaderResource& resource);

		static void bindTexture(Texture& texture);

		static void bindShader(Shader& shader);

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		static void setDefaultRenderTarget(RenderTarget& renderTarget);
		static void setRenderTarget(RenderTarget& renderTarget);
		static void resetRenderTargetToDefault();
		//

		static void setViewport(const Viewport& viewport);

		static void clear();
		static void drawIndexed(const uint32 count);

		static void setClearColour(const math::Vector4f& colour);
		static void setDepthBuffer(bool enabled);
		static void setBlendState(bool enabled);

		//TEMP: Need an 'unbind' function (perhaps this will be solved the pipeline object?)
		static void removeCurrentGeometryShader();
		static void removeTextureAtSlot(uint32 slot);
		//

		static std::shared_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor, void* indices);
		static std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor);

		static std::shared_ptr<ShaderResource> createShaderResource(const ShaderResourceDescriptor& descriptor);

		static std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor);

		static std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor);

		static std::shared_ptr<Surface> createSurface();

		static void initialise(gfx::API api);
	};
}
