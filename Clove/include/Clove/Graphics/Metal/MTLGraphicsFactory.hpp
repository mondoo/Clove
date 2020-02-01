#pragma once

#include "Clove/Graphics/Core/GraphicsFactory.hpp"

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
		
		virtual ~MTLGraphicsFactory();
		
		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(const std::shared_ptr<RenderTarget>& renderTarget) override;
		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(Surface& surface) override;

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP) override;

		virtual std::shared_ptr<PipelineObject> createPipelineObject() override;

		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor, std::string_view pathToShader) override;

		virtual std::shared_ptr<Surface> createSurface(void* windowData) override;
	};
}
