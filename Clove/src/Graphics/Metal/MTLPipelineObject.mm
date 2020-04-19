#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"

#include "Clove/Graphics/Metal/MTLShader.hpp"

namespace clv::gfx::mtl{
	MTLPipelineObject::MTLPipelineObject(std::shared_ptr<GraphicsFactory> factory)
		: factory(std::move(factory)) {
		pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
		pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		[pipelineDescriptor.colorAttachments[0] setDestinationRGBBlendFactor:MTLBlendFactorOneMinusSourceAlpha];
		
		pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
		
		setBlendState(true);
		setCullMode(CullFace::Back, true);
	}
	
	MTLPipelineObject::MTLPipelineObject(MTLPipelineObject&& other) noexcept = default;
	
	MTLPipelineObject& MTLPipelineObject::operator=(MTLPipelineObject&& other) noexcept = default;
	
	MTLPipelineObject::~MTLPipelineObject() = default;
	
	const std::shared_ptr<GraphicsFactory>& getFactory() const {
		return factory;
	}

	void MTLPipelineObject::setVertexShader(const Shader& vertexShader){
		const MTLShader& mtlShader = static_cast<const MTLShader&>(vertexShader);
		id<MTLFunction> shaderFunction = mtlShader.getMTLShader();
		[pipelineDescriptor setVertexFunction:shaderFunction];
		
		vertexLayout.clear();
		NSArray<MTLVertexAttribute*>* attributes = shaderFunction.vertexAttributes;
		for(NSInteger i = 0; i < attributes.count; ++i){
			vertexLayout.add(VertexElement::getTypeFromSemantic([attributes[i].name cStringUsingEncoding:[NSString defaultCStringEncoding]]));
		}
		
		MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor alloc] init];
		NSUInteger offset = 0;
		for(int32_t i = 0; i < vertexLayout.count(); ++i){
			const auto& element = vertexLayout.resolve(i);
			const VertexElementType elementType = element.getType();
			
			vertexDescriptor.attributes[i].format = getMTLFormatFromType(elementType);
			vertexDescriptor.attributes[i].offset = offset;
			vertexDescriptor.attributes[i].bufferIndex = 0;
			
			offset += VertexElement::sizeOf(elementType);
		}
		vertexDescriptor.layouts[0].stride = vertexLayout.size();
		
		[pipelineDescriptor setVertexDescriptor:vertexDescriptor];
	}
	
	void MTLPipelineObject::setGeometryShader(const Shader& geometryShader){
		CLV_ASSERT(false, "Geometry shader not supported on Metal!");
	}
	
	void MTLPipelineObject::setPixelShader(const Shader& pixelShader){
		const MTLShader& mtlShader = static_cast<const MTLShader&>(pixelShader);
		[pipelineDescriptor setFragmentFunction:mtlShader.getMTLShader()];
	}
	
	void MTLPipelineObject::setBlendState(bool enabled){
		[pipelineDescriptor.colorAttachments[0] setBlendingEnabled:(enabled ? YES : NO)];
	}
	
	void MTLPipelineObject::setCullMode(CullFace face, bool frontFaceCounterClockwise){
		cullFace = face;
		this->frontFaceCounterClockwise = frontFaceCounterClockwise;
	}
	
	const VertexLayout& MTLPipelineObject::getVertexLayout() const{
		return vertexLayout;
	}
	
	MTLRenderPipelineDescriptor* MTLPipelineObject::getMTLPipelineStateDescriptor() const{
		return pipelineDescriptor;
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
			case VertexElementType::colour3D:
				return MTLVertexFormatFloat3;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return MTLVertexFormatFloat3;
		}
	}
}
