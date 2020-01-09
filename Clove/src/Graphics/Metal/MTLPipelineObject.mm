#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"

#include "Clove/Graphics/Metal/MTLShader.hpp"

namespace clv::gfx::mtl{
	MTLPipelineObject::MTLPipelineObject(id<MTLDevice> mtlDevice, const std::shared_ptr<Shader>& shader){
		const auto mtlShader = std::static_pointer_cast<MTLShader>(shader);
		
		shaderReflectionData = mtlShader->getReflectionData();
		const auto& layout = shaderReflectionData.vertexBufferLayout;
		
		pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
		
		[pipelineDescriptor setVertexFunction:mtlShader->getMTLVertexShader()];
		[pipelineDescriptor setFragmentFunction:mtlShader->getMTLPixelShader()];
		
		pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		[pipelineDescriptor.colorAttachments[0] setDestinationRGBBlendFactor:MTLBlendFactorOneMinusSourceAlpha];
		
		pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
		
		MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor alloc] init];
		
		NSUInteger offset = 0;
		for(int32 i = 0; i < layout.count(); ++i){
			const auto& element = layout.resolve(i);
			const VertexElementType elementType = element.getType();
			
			vertexDescriptor.attributes[i].format = getMTLFormatFromType(elementType);
			vertexDescriptor.attributes[i].offset = offset;
			vertexDescriptor.attributes[i].bufferIndex = 0;
			
			offset += VertexElement::sizeOf(elementType);
		}
		vertexDescriptor.layouts[0].stride = layout.size();
		
		[pipelineDescriptor setVertexDescriptor:vertexDescriptor];

		NSError *error;
		pipelineState = [mtlDevice newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
		
		[vertexDescriptor release];
		[error release];
	}
	
	MTLPipelineObject::MTLPipelineObject(MTLPipelineObject&& other) noexcept = default;
	
	MTLPipelineObject& MTLPipelineObject::operator=(MTLPipelineObject&& other) noexcept = default;
	
	MTLPipelineObject::~MTLPipelineObject(){
		[pipelineState release];
	}
	
	void MTLPipelineObject::setBlendState(bool enabled){
		[pipelineDescriptor.colorAttachments[0] setBlendingEnabled:(enabled ? YES : NO)];
	}
	
	void MTLPipelineObject::setCullMode(CullFace face, bool frontFaceCounterClockwise){
		cullFace = face;
		this->frontFaceCounterClockwise = frontFaceCounterClockwise;
	}
	
	const std::shared_ptr<Shader>& MTLPipelineObject::getShader() const{
		return shader;
	}
	
	const VertexLayout& MTLPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
	}
	
	id<MTLRenderPipelineState> MTLPipelineObject::getMTLPipelineState() const{
		return pipelineState;
	}
	
	CullFace MTLPipelineObject::getCullFace() const{
		return cullFace;
	}
	
	bool MTLPipelineObject::isFrontFaceCounterClockWise() const{
		return frontFaceCounterClockwise;
	}
	
	MTLVertexFormat MTLPipelineObject::getMTLFormatFromType(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return MTLVertexFormatFloat2;
			case VertexElementType::position3D:
				return MTLVertexFormatFloat3;
			case VertexElementType::texture2D:
				return MTLVertexFormatFloat2;
			case VertexElementType::normal:
				return MTLVertexFormatFloat3;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return MTLVertexFormatFloat3;
		}
	}
}
