#pragma once

#include "Clove/Core/Graphics/RenderFactory.hpp"

namespace clv::gfx::mtl{
	class MTLRenderFactory : public RenderFactory{
		//VARIABLES
	private:
		id<MTLDevice> mtlDevice;
		
		//FUNCTIONS
	public:
		MTLRenderFactory() = delete;
		MTLRenderFactory(id<MTLDevice> mtlDevice);
						 
		//TODO: Ctors
		
		virtual ~MTLRenderFactory();

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP) override;
		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) override;
		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;
		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;
		virtual std::shared_ptr<Surface> createSurface(void* windowData) override;
	};
}
