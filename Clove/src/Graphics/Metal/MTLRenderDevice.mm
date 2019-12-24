#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/MTLRenderDevice.hpp"

namespace clv::gfx::mtl{
	void MTLRenderDevice::bindIndexBuffer(const Buffer& buffer){
		//TODO:
	}
	
	void MTLRenderDevice::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		//TODO:
	}
	
	void MTLRenderDevice::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		//TODO:
	}
	
	void MTLRenderDevice::bindPipelineObject(const PipelineObject& pipelineObject){
		//TODO:
	}
	
	void MTLRenderDevice::bindTexture(const Texture* texture, const uint32 bindingPoint){
		//TODO:
	}
	
	void MTLRenderDevice::bindShader(const Shader& shader){
		//TODO:
	}
	
	void MTLRenderDevice::updateBufferData(Buffer& buffer, const void* data){
		//TODO:
	}

	void MTLRenderDevice::makeSurfaceCurrent(const std::shared_ptr<Surface>& surface){
		//TODO:
	}

	void MTLRenderDevice::setRenderTarget(const RenderTarget* renderTarget){
		//TODO:
	}
	
	void MTLRenderDevice::resetRenderTargetToDefault(){
		//TODO:
	}

	void MTLRenderDevice::setViewport(const Viewport& viewport){
		//TODO:
	}

	void MTLRenderDevice::clear(){
		//TODO:
	}
	
	void MTLRenderDevice::drawIndexed(const uint32 count){
		//TODO:
	}

	void MTLRenderDevice::setClearColour(const mth::vec4f& colour){
		//TODO:
	}
	
	void MTLRenderDevice::setDepthBuffer(bool enabled){
		//TODO:
	}
	
	void MTLRenderDevice::setBlendState(bool enabled){
		//TODO:
	}

}

