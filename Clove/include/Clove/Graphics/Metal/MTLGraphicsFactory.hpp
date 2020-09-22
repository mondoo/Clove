#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"

#import <MetalKit/MetalKit.h>

#include <span>

namespace clv::gfx::mtl{
	class MTLGraphicsFactory : public GraphicsFactory{
		//VARIABLES
	private:
		id<MTLDevice> mtlDevice;
		
		//FUNCTIONS
	public:
		MTLGraphicsFactory() = delete;
		MTLGraphicsFactory(id<MTLDevice> mtlDevice);
						 
		MTLGraphicsFactory(const MTLGraphicsFactory& other) = delete;
		MTLGraphicsFactory(MTLGraphicsFactory&& other) noexcept = delete;

		MTLGraphicsFactory& operator=(const MTLGraphicsFactory& other) = delete;
		MTLGraphicsFactory& operator=(MTLGraphicsFactory&& other) noexcept = delete;
		
		~MTLGraphicsFactory();
		
		std::shared_ptr<CommandBuffer> createCommandBuffer() override;

		std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP) override;

		std::shared_ptr<PipelineObject> createPipelineObject() override;

		std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, std::string_view pathToShader) override;
		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, std::span<const std::byte> sourceBytes) override;

		std::shared_ptr<Surface> createSurface(void* windowData) override;
	};
}
