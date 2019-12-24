#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/MTLRenderFactory.hpp"

#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"
//#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"
//#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"
#include "Clove/Graphics/Metal/MTLShader.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

namespace clv::gfx::mtl{
	MTLRenderFactory::MTLRenderFactory(id<MTLDevice> mtlDevice)
		: mtlDevice(mtlDevice){
	}
	
	MTLRenderFactory::~MTLRenderFactory(){
		[mtlDevice release];
	}
	
	std::shared_ptr<Buffer> MTLRenderFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<MTLBuffer>(descriptor, data);
	}
	
	std::shared_ptr<Texture> MTLRenderFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<MTLTexture>(descriptor, pathToTexture);
	}
	
	std::shared_ptr<Texture> MTLRenderFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP){
		return std::make_shared<MTLTexture>(descriptor, data, BPP);
	}
	
	std::shared_ptr<PipelineObject> MTLRenderFactory::createPipelineObject(const std::shared_ptr<Shader>& shader){
		//TODO:
	}
	
	std::shared_ptr<RenderTarget> MTLRenderFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		//TODO:
	}
	
	std::shared_ptr<Shader> MTLRenderFactory::createShader(const ShaderDescriptor& descriptor){
		return std::make_shared<MTLShader>(descriptor);
	}
	
	std::shared_ptr<Surface> MTLRenderFactory::createSurface(void* windowData){
		return std::make_shared<MTLSurface>(descriptor);
	}
}
