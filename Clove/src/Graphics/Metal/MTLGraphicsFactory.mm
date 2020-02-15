#include "Clove/Graphics/Metal/MTLGraphicsFactory.hpp"

#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"
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
		return std::make_shared<MTLCommandBuffer>([mtlDevice newCommandQueue]);
	}
	
	std::shared_ptr<Buffer> MTLGraphicsFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<MTLBuffer>(mtlDevice, descriptor, data);
	}
	
	std::shared_ptr<Texture> MTLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<MTLTexture>(mtlDevice, descriptor, pathToTexture);
	}
	
	std::shared_ptr<Texture> MTLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP){
		return std::make_shared<MTLTexture>(mtlDevice, descriptor, data, BPP);
	}
	
	std::shared_ptr<PipelineObject> MTLGraphicsFactory::createPipelineObject(){
		return std::make_shared<MTLPipelineObject>();
	}
	
	std::shared_ptr<RenderTarget> MTLGraphicsFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<MTLRenderTarget>(mtlDevice, colourTexture, depthStencilTexture);
	}
	
	std::shared_ptr<Shader> MTLGraphicsFactory::createShader(const ShaderDescriptor& descriptor, std::string_view pathToShader){
		return std::make_shared<MTLShader>(mtlDevice, descriptor, pathToShader);
	}
	
	std::shared_ptr<Surface> MTLGraphicsFactory::createSurface(void* windowData){
		return std::make_shared<MTLSurface>(mtlDevice, windowData);
	}
}
