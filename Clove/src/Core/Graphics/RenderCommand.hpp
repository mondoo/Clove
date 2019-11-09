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
		static void bindIndexBuffer(const Buffer& buffer);
		static void bindVertexBuffer(const Buffer& buffer);
		static void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint);
		static void bindTexture(const Texture& texture, const uint32 bindingPoint);
		static void bindShader(const Shader& shader);

		static void updateBufferData(Buffer& buffer, void* data);

		static void makeSurfaceCurrent(Surface& surface);

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		static void setRenderTarget(RenderTarget& renderTarget);
		static void resetRenderTargetToDefault();
		//

		static void setViewport(const Viewport& viewport);

		static void clear();
		static void drawIndexed(const uint32 count);

		static void setClearColour(const math::Vector4f& colour);
		static void setDepthBuffer(bool enabled);
		static void setBlendState(bool enabled);

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		static void removeCurrentGeometryShader();
		static void removeTextureAtSlot(uint32 slot);
		//

		static std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, void* data);
		static std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture);
		static std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, void* data, int32 BPP);
		static std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture);
		static std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor);
		static std::shared_ptr<Surface> createSurface();

		static void initialise(gfx::API api);
	};
}
