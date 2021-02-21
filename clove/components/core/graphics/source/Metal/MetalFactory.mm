#include "Clove/Graphics/Metal/MetalFactory.hpp"

#include "Clove/Graphics/ShaderCompiler.hpp"
#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalShader.hpp"
#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"

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
	}
	MetalFactory::MetalFactory(id<MTLDevice> device)
		: device{ device }{
	}
	
	MetalFactory::MetalFactory(MetalFactory &&other) noexcept = default;

	MetalFactory &MetalFactory::operator=(MetalFactory &&other) noexcept = default;

	MetalFactory::~MetalFactory() = default;

	Expected<std::unique_ptr<GhaGraphicsQueue>, std::runtime_error> MetalFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
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
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaDescriptorSetLayout>, std::runtime_error> MetalFactory::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaGraphicsPipelineObject>, std::runtime_error> MetalFactory::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
		MetalRenderPass *metalRenderPass{ polyCast<MetalRenderPass>(descriptor.renderPass.get()) };
		
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
		
		//View / scissor
		//TODO:
		
		//Rasteriser
		//TODO:
		
		//Depth state
		//TODO:
		
		//Blending
		//TODO:
		
		//Render pass
		//TODO:
		
		//Descriptors
		//TODO:
		
		//Push constants
		//TODO:
		
		MTLRenderPipelineDescriptor *pipelineDesc{ [[MTLRenderPipelineDescriptor alloc] init] };
		pipelineDesc.vertexFunction = vertexFunction;
		pipelineDesc.fragmentFunction = fragmentFunction;
		pipelineDesc.vertexDescriptor = vertexDescriptor;
		
		NSError *error{ nullptr };
		id<MTLRenderPipelineState> pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];
		
		if(error != nullptr && error.code != 0){
			return Unexpected{ std::runtime_error{ [[error description] cStringUsingEncoding:[NSString defaultCStringEncoding]] } };
		}
		
		return std::unique_ptr<GhaGraphicsPipelineObject>{ std::make_unique<MetalGraphicsPipelineObject>(pipelineState) };
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
		return Unexpected{ std::runtime_error{ "Not implemented" } };
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
