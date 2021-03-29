#include "Clove/Graphics/Metal/MetalFactory.hpp"

#include "Clove/Graphics/ShaderCompiler.hpp"
#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalShader.hpp"
#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"
#include "Clove/Graphics/Metal/MetalGraphicsQueue.hpp"
#include "Clove/Graphics/Metal/MetalImage.hpp"
#include "Clove/Graphics/Metal/MetalRenderPass.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	namespace {
		MTLVertexFormat convertAttributeFormat(VertexAttributeFormat garlicFormat) {
			switch(garlicFormat) {
				case VertexAttributeFormat::R32_SFLOAT:
					return MTLVertexFormatFloat;
				case VertexAttributeFormat::R32G32_SFLOAT:
					return MTLVertexFormatFloat2;
				case VertexAttributeFormat::R32G32B32_SFLOAT:
					return MTLVertexFormatFloat3;
				case VertexAttributeFormat::R32G32B32A32_SFLOAT:
					return MTLVertexFormatFloat4;
				case VertexAttributeFormat::R32G32B32A32_SINT:
					return MTLVertexFormatInt4;
				default:
					CLOVE_ASSERT(false, "{0}: Unkown format passed", CLOVE_FUNCTION_NAME_PRETTY);
					return MTLVertexFormatFloat;
			}
		}
		
		MTLTextureType convertImageType(MetalImage::Type type) {
			switch (type) {
				case MetalImage::Type::_2D:
					return MTLTextureType2D;
				case MetalImage::Type::_3D:
					return MTLTextureType3D;
				case MetalImage::Type::Cube:
					return MTLTextureTypeCube;
				default:
					CLOVE_ASSERT(false, "{0}: Unkown type passed", CLOVE_FUNCTION_NAME_PRETTY);
					return MTLTextureType2D;
			}
		}
		
		MTLTextureUsage getUsageFlags(GhaImage::UsageMode flags) {
			MTLTextureUsage mtlFlags{ MTLTextureUsageUnknown };
			
			if((flags & GhaImage::UsageMode::TransferSource) != 0) {
				mtlFlags |= MTLTextureUsageUnknown;
			}
			if((flags & GhaImage::UsageMode::TransferDestination) != 0) {
				mtlFlags |= MTLTextureUsageUnknown;
			}
			if((flags & GhaImage::UsageMode::Sampled) != 0) {
				mtlFlags |= MTLTextureUsageShaderRead;
			}
			if((flags & GhaImage::UsageMode::ColourAttachment) != 0) {
				mtlFlags |= MTLTextureUsageRenderTarget;
			}
			if((flags & GhaImage::UsageMode::DepthStencilAttachment) != 0) {
				mtlFlags |= MTLTextureUsageRenderTarget;
			}
			
			return mtlFlags;
		}
	}
	
	MetalFactory::MetalFactory(id<MTLDevice> device)
		: device{ device }{
	}
	
	MetalFactory::MetalFactory(MetalFactory &&other) noexcept = default;

	MetalFactory &MetalFactory::operator=(MetalFactory &&other) noexcept = default;

	MetalFactory::~MetalFactory() = default;

	Expected<std::unique_ptr<GhaGraphicsQueue>, std::runtime_error> MetalFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) {
		return std::unique_ptr<GhaGraphicsQueue>{ std::make_unique<MetalGraphicsQueue>([device newCommandQueue]) };
	}
	
	Expected<std::unique_ptr<GhaPresentQueue>, std::runtime_error> MetalFactory::createPresentQueue() {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaTransferQueue>, std::runtime_error> MetalFactory::createTransferQueue(CommandQueueDescriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaComputeQueue>, std::runtime_error> MetalFactory::createComputeQueue(CommandQueueDescriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaSwapchain>, std::runtime_error> MetalFactory::createSwapChain(GhaSwapchain::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaShader>, std::runtime_error> MetalFactory::createShaderFromFile(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
		Expected<std::vector<uint32_t>, std::runtime_error> compilationResult{ ShaderCompiler::compileFromFile(file, shaderStage) };
		if(compilationResult.hasValue()) {
			Expected<std::string, std::runtime_error> msl{ ShaderCompiler::spirvToMSL(compilationResult.getValue()) };
			if(msl.hasValue()) {
				return createShaderObject(std::move(msl.getValue()));
			} else {
				return Unexpected{ std::move(msl.getError()) };
			}
		} else {
			return Unexpected{ std::move(compilationResult.getError()) };
		}
	}
	
	Expected<std::unique_ptr<GhaShader>, std::runtime_error> MetalFactory::createShaderFromSource(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
		Expected<std::vector<uint32_t>, std::runtime_error> compilationResult{ ShaderCompiler::compileFromSource(source, std::move(includeSources), shaderName, shaderStage) };
		if(compilationResult.hasValue()) {
			Expected<std::string, std::runtime_error> msl{ ShaderCompiler::spirvToMSL(compilationResult.getValue()) };
			if(msl.hasValue()) {
				return createShaderObject(std::move(msl.getValue()));
			} else {
				return Unexpected{ std::move(msl.getError()) };
			}
		} else {
			return Unexpected{ std::move(compilationResult.getError()) };
		}
	}

	Expected<std::unique_ptr<GhaRenderPass>, std::runtime_error> MetalFactory::createRenderPass(GhaRenderPass::Descriptor descriptor) {
		MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments{};
		for(size_t i{ 0 }; i < descriptor.colourAttachments.size(); ++i){
			colourAttachments[i].pixelFormat = MetalImage::convertFormat(descriptor.colourAttachments[i].format);
		}
		
		MTLPixelFormat depthPixelFormat{ MetalImage::convertFormat(descriptor.depthAttachment.format) };
		
		return std::unique_ptr<GhaRenderPass>{ std::make_unique<MetalRenderPass>(std::move(colourAttachments), std::move(depthPixelFormat), std::move(descriptor)) };
	}
	
	Expected<std::unique_ptr<GhaDescriptorSetLayout>, std::runtime_error> MetalFactory::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaGraphicsPipelineObject>, std::runtime_error> MetalFactory::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
		//Vertex shader
		id<MTLFunction> vertexFunction{ polyCast<MetalShader>(descriptor.vertexShader.get())->getFunction() };
		
		//Pixel shader
		id<MTLFunction> fragmentFunction{ polyCast<MetalShader>(descriptor.pixelShader.get())->getFunction() };
		
		//Vertex input
		MTLVertexDescriptor *vertexDescriptor{ [[MTLVertexDescriptor alloc] init] };
		vertexDescriptor.layouts[0].stride = descriptor.vertexInput.stride;
		for(size_t i{0}; i < descriptor.vertexAttributes.size(); ++i){
			auto const &attribute{ descriptor.vertexAttributes[i] };
			vertexDescriptor.attributes[i].bufferIndex = 0;
			vertexDescriptor.attributes[i].format = convertAttributeFormat(attribute.format);
			vertexDescriptor.attributes[i].offset = attribute.offset;
		}
		
		//Topology
		MTLPrimitiveTopologyClass const topology{ MTLPrimitiveTopologyClassTriangle };
		
		//Depth state
		MTLDepthStencilDescriptor *depthStencil{ [[MTLDepthStencilDescriptor alloc] init]};
		depthStencil.depthWriteEnabled = descriptor.depthState.depthWrite;
		if(descriptor.depthState.depthTest){
			depthStencil.depthCompareFunction = MTLCompareFunctionLess;
		}else{
			depthStencil.depthCompareFunction = MTLCompareFunctionAlways;
		}
		
		id<MTLDepthStencilState> depthStencilState{ [device newDepthStencilStateWithDescriptor:depthStencil] };
		
		//Render pass
		MetalRenderPass const *const renderPass{ polyCast<MetalRenderPass>(descriptor.renderPass.get()) };
		size_t const colourAttachmentCount{ renderPass->getDescriptor().colourAttachments.size() };
		MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments{ renderPass->getColourAttachments() };
		MTLPixelFormat const depthPixelFormat{ renderPass->getDepthPixelFormat() };
		
		//Descriptors
		//TODO:
		
		//Push constants
		//TODO:
		
		MTLRenderPipelineDescriptor *pipelineDesc{ [[MTLRenderPipelineDescriptor alloc] init] };
		pipelineDesc.vertexFunction = vertexFunction;
		pipelineDesc.fragmentFunction = fragmentFunction;
		pipelineDesc.vertexDescriptor = vertexDescriptor;
		pipelineDesc.inputPrimitiveTopology = topology;
		for(size_t i{ 0 }; i < colourAttachmentCount; ++i){
			pipelineDesc.colorAttachments[i] = colourAttachments[i];
			
			//Blending
			pipelineDesc.colorAttachments[i].blendingEnabled = descriptor.enableBlending;
			pipelineDesc.colorAttachments[i].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
			pipelineDesc.colorAttachments[i].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
			pipelineDesc.colorAttachments[i].rgbBlendOperation = MTLBlendOperationAdd;
			pipelineDesc.colorAttachments[i].sourceAlphaBlendFactor = MTLBlendFactorOne;
			pipelineDesc.colorAttachments[i].destinationAlphaBlendFactor = MTLBlendFactorZero;
			pipelineDesc.colorAttachments[i].alphaBlendOperation = MTLBlendOperationAdd;
		}
		pipelineDesc.depthAttachmentPixelFormat = depthPixelFormat;
		
		NSError *error{ nullptr };
		id<MTLRenderPipelineState> pipelineState{ [device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error] };
		if(error != nullptr && error.code != 0){
			return Unexpected{ std::runtime_error{ [[error description] cStringUsingEncoding:[NSString defaultCStringEncoding]] } };
		}
		
		return std::unique_ptr<GhaGraphicsPipelineObject>{ std::make_unique<MetalGraphicsPipelineObject>(pipelineState, depthStencilState) };
	}
	
	Expected<std::unique_ptr<GhaComputePipelineObject>, std::runtime_error> MetalFactory::createComputePipelineObject(GhaComputePipelineObject::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaFramebuffer>, std::runtime_error> MetalFactory::createFramebuffer(GhaFramebuffer::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaDescriptorPool>, std::runtime_error> MetalFactory::createDescriptorPool(GhaDescriptorPool::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaSemaphore>, std::runtime_error> MetalFactory::createSemaphore() {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaFence>, std::runtime_error> MetalFactory::createFence(GhaFence::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaBuffer>, std::runtime_error> MetalFactory::createBuffer(GhaBuffer::Descriptor descriptor) {
		MTLResourceOptions resourceOptions{};
		
		//Usage mode -- not needed
		//Sharing mode -- not needed
		
		switch (descriptor.memoryType) {
			case MemoryType::VideoMemory:
				resourceOptions |= MTLResourceStorageModePrivate;
				break;
			case MemoryType::SystemMemory:
				resourceOptions |= MTLResourceStorageModeShared | MTLResourceCPUCacheModeDefaultCache;//Including MTLResourceCPUCacheModeDefaultCache here as this makes mapping memory more simple
				break;
			default:
				break;
		}
		
		id<MTLBuffer> buffer{ [device newBufferWithLength:descriptor.size options:resourceOptions] };
		
		return std::unique_ptr<GhaBuffer>{ std::make_unique<MetalBuffer>(buffer, std::move(descriptor)) };
	}
	
	Expected<std::unique_ptr<GhaImage>, std::runtime_error> MetalFactory::createImage(GhaImage::Descriptor descriptor) {
		MTLTextureDescriptor *mtlDescriptor;
		
		mtlDescriptor.textureType = convertImageType(descriptor.type);
		mtlDescriptor.pixelFormat = MetalImage::convertFormat(descriptor.format);
		mtlDescriptor.width = descriptor.dimensions.x;
		mtlDescriptor.height = descriptor.dimensions.y;
		mtlDescriptor.resourceOptions = MTLResourceStorageModePrivate;
		mtlDescriptor.usage = getUsageFlags(descriptor.usageFlags);
		
		id<MTLTexture> texture{ [device newTextureWithDescriptor:mtlDescriptor] };
		
		return std::unique_ptr<GhaImage>{ std::make_unique<MetalImage>(texture, std::move(descriptor)) };
	}

	Expected<std::unique_ptr<GhaSampler>, std::runtime_error> MetalFactory::createSampler(GhaSampler::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaShader>, std::runtime_error> MetalFactory::createShaderObject(std::string mslSource) {
		NSError *libError;
		id<MTLLibrary> library{ [device newLibraryWithSource:[NSString stringWithCString:mslSource.c_str() encoding:[NSString defaultCStringEncoding]] options:nil error:&libError] };
		
		if(libError != nullptr && libError.code != 0){
			return Unexpected{ std::runtime_error{ [[libError description] cStringUsingEncoding:[NSString defaultCStringEncoding]] } };
		}
		
		return std::unique_ptr<GhaShader>{ std::make_unique<MetalShader>([library newFunctionWithName:@"main"]) };
	}
}
