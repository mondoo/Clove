#include "Clove/Graphics/Metal/MetalFactory.hpp"

#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalShader.hpp"

namespace garlic::clove {
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
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaShader>, std::runtime_error> MetalFactory::createShaderFromSource(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaRenderPass>, std::runtime_error> MetalFactory::createRenderPass(GhaRenderPass::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}
	
	Expected<std::unique_ptr<GhaDescriptorSetLayout>, std::runtime_error> MetalFactory::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
	}

	Expected<std::unique_ptr<GhaGraphicsPipelineObject>, std::runtime_error> MetalFactory::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
		return Unexpected{ std::runtime_error{ "Not implemented" } };
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
