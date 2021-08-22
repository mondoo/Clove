#include "Clove/Graphics/Validation/ValidationFactory.hpp"

#include "Clove/Graphics/Validation/ValidationBuffer.hpp"

namespace clove {
    ValidationFactory::ValidationFactory(GhaFactory *factory)
        : internalFactory{ factory } {
    }

    ValidationFactory::ValidationFactory(ValidationFactory &&other) noexcept = default;

    ValidationFactory &ValidationFactory::operator=(ValidationFactory &&other) noexcept = default;

    ValidationFactory::~ValidationFactory() = default;

    Expected<std::unique_ptr<GhaGraphicsQueue>, std::runtime_error> ValidationFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) noexcept {
        return internalFactory->createGraphicsQueue(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaPresentQueue>, std::runtime_error> ValidationFactory::createPresentQueue() noexcept {
        return internalFactory->createPresentQueue();
    }

    Expected<std::unique_ptr<GhaTransferQueue>, std::runtime_error> ValidationFactory::createTransferQueue(CommandQueueDescriptor descriptor) noexcept {
        return internalFactory->createTransferQueue(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaComputeQueue>, std::runtime_error> ValidationFactory::createComputeQueue(CommandQueueDescriptor descriptor) noexcept {
        return internalFactory->createComputeQueue(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaSwapchain>, std::runtime_error> ValidationFactory::createSwapChain(GhaSwapchain::Descriptor descriptor) noexcept {
        return internalFactory->createSwapChain(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaShader>, std::runtime_error> ValidationFactory::createShaderFromFile(std::filesystem::path const &file, GhaShader::Stage shaderStage) noexcept {
        return internalFactory->createShaderFromFile(file, shaderStage);
    }

    Expected<std::unique_ptr<GhaShader>, std::runtime_error> ValidationFactory::createShaderFromSource(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) noexcept {
        return internalFactory->createShaderFromSource(source, std::move(includeSources), shaderName, shaderStage);
    }

    Expected<std::unique_ptr<GhaRenderPass>, std::runtime_error> ValidationFactory::createRenderPass(GhaRenderPass::Descriptor descriptor) noexcept {
        return internalFactory->createRenderPass(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaDescriptorSetLayout>, std::runtime_error> ValidationFactory::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) noexcept {
        return internalFactory->createDescriptorSetLayout(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaGraphicsPipelineObject>, std::runtime_error> ValidationFactory::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) noexcept {
        return internalFactory->createGraphicsPipelineObject(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaComputePipelineObject>, std::runtime_error> ValidationFactory::createComputePipelineObject(GhaComputePipelineObject::Descriptor descriptor) noexcept {
        return internalFactory->createComputePipelineObject(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaFramebuffer>, std::runtime_error> ValidationFactory::createFramebuffer(GhaFramebuffer::Descriptor descriptor) noexcept {
        return internalFactory->createFramebuffer(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaDescriptorPool>, std::runtime_error> ValidationFactory::createDescriptorPool(GhaDescriptorPool::Descriptor descriptor) noexcept {
        return internalFactory->createDescriptorPool(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaSemaphore>, std::runtime_error> ValidationFactory::createSemaphore() noexcept {
        return internalFactory->createSemaphore();
    }

    Expected<std::unique_ptr<GhaFence>, std::runtime_error> ValidationFactory::createFence(GhaFence::Descriptor descriptor) noexcept {
        return internalFactory->createFence(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaBuffer>, std::runtime_error> ValidationFactory::createBuffer(GhaBuffer::Descriptor descriptor) noexcept {
        auto result{ internalFactory->createBuffer(descriptor) };
        if(result.hasValue()) {
            return std::unique_ptr<GhaBuffer>{ std::make_unique<ValidationBuffer>(std::move(descriptor), std::move(result.getValue())) };
        } else {
            return result;
        }
    }

    Expected<std::unique_ptr<GhaImage>, std::runtime_error> ValidationFactory::createImage(GhaImage::Descriptor descriptor) noexcept {
        return internalFactory->createImage(std::move(descriptor));
    }

    Expected<std::unique_ptr<GhaSampler>, std::runtime_error> ValidationFactory::createSampler(GhaSampler::Descriptor descriptor) noexcept {
        return internalFactory->createSampler(std::move(descriptor));
    }
}