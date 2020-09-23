#include "Clove/Graphics/Metal/MTLGraphicsFactory.hpp"

#include "Clove/Graphics/Metal/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/MTLTexture.hpp"
#include "Clove/Graphics/Metal/MTLCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"
#include "Clove/Graphics/Metal/MTLShader.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

namespace clv::gfx::mtl{
	MTLGraphicsFactory::MTLGraphicsFactory(id<MTLDevice> mtlDevice){
		this->mtlDevice = [mtlDevice retain];
	}
	
	MTLGraphicsFactory::~MTLGraphicsFactory(){
		[mtlDevice release];
	}
	
	std::shared_ptr<CommandBuffer> MTLGraphicsFactory::createCommandBuffer(){
		return std::make_shared<MTLCommandBuffer>(shared_from_this(), [mtlDevice newCommandQueue]);
	}
	
	std::shared_ptr<Buffer> MTLGraphicsFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<MTLBuffer>(shared_from_this(), mtlDevice, descriptor, data);
	}
	
	std::shared_ptr<Texture> MTLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<MTLTexture>(shared_from_this(), mtlDevice, descriptor, pathToTexture);
	}
	
	std::shared_ptr<Texture> MTLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP){
		return std::make_shared<MTLTexture>(shared_from_this(), mtlDevice, descriptor, data, BPP);
	}
	
	std::shared_ptr<PipelineObject> MTLGraphicsFactory::createPipelineObject(){
		return std::make_shared<MTLPipelineObject>(shared_from_this());
	}
	
	std::shared_ptr<RenderTarget> MTLGraphicsFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<MTLRenderTarget>(shared_from_this(), mtlDevice, colourTexture, depthStencilTexture);
	}
	
	std::shared_ptr<Shader> MTLGraphicsFactory::createShader(ShaderDescriptor descriptor, std::string_view pathToShader){
		return std::make_shared<MTLShader>(shared_from_this(), mtlDevice, std::move(descriptor), pathToShader);
	}

	std::shared_ptr<Shader> MTLGraphicsFactory::createShader(ShaderDescriptor descriptor, std::span<const std::byte> sourceBytes){
		return std::make_shared<MTLShader>(shared_from_this(), mtlDevice, std::move(descriptor), std::move(sourceBytes));
	}
	
	std::shared_ptr<Surface> MTLGraphicsFactory::createSurface(void* windowData){
		return std::make_shared<MTLSurface>(shared_from_this(), mtlDevice, windowData);
	}
}
