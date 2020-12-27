#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <optional>
#include <span>
#include <vulkan/vulkan.h>

namespace garlic::clove {
    class MemoryAllocator;
}

namespace garlic::clove {
    class VKGraphicsFactory : public GraphicsFactory {
        //VARIABLES
    private:
        DevicePointer devicePtr;
        QueueFamilyIndices queueFamilyIndices;

        std::shared_ptr<MemoryAllocator> memoryAllocator;

        //FUNCTIONS
    public:
        VKGraphicsFactory() = delete;
        VKGraphicsFactory(DevicePointer devicePtr, QueueFamilyIndices queueFamilyIndices);

        VKGraphicsFactory(VKGraphicsFactory const &other) = delete;
        VKGraphicsFactory(VKGraphicsFactory &&other) noexcept;

        VKGraphicsFactory &operator=(VKGraphicsFactory const &other) = delete;
        VKGraphicsFactory &operator                                  =(VKGraphicsFactory &&other) noexcept;

        ~VKGraphicsFactory();

        Expected<std::unique_ptr<GraphicsQueue>, std::runtime_error> createGraphicsQueue(CommandQueueDescriptor descriptor) override;
        Expected<std::unique_ptr<PresentQueue>, std::runtime_error> createPresentQueue() override;
        Expected<std::unique_ptr<TransferQueue>, std::runtime_error> createTransferQueue(CommandQueueDescriptor descriptor) override;

        Expected<std::unique_ptr<Swapchain>, std::runtime_error> createSwapChain(Swapchain::Descriptor descriptor) override;

        Expected<std::unique_ptr<Shader>, std::runtime_error> createShader(std::string_view filePath, Shader::Stage shaderStage) override;
        Expected<std::unique_ptr<Shader>, std::runtime_error> createShader(std::span<std::byte> source, Shader::Stage shaderStage) override;

        Expected<std::unique_ptr<RenderPass>, std::runtime_error> createRenderPass(RenderPass::Descriptor descriptor) override;
        Expected<std::unique_ptr<DescriptorSetLayout>, std::runtime_error> createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) override;

        Expected<std::unique_ptr<PipelineObject>, std::runtime_error> createPipelineObject(PipelineObject::Descriptor descriptor) override;

        Expected<std::unique_ptr<Framebuffer>, std::runtime_error> createFramebuffer(Framebuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<DescriptorPool>, std::runtime_error> createDescriptorPool(DescriptorPool::Descriptor descriptor) override;

        Expected<std::unique_ptr<Semaphore>, std::runtime_error> createSemaphore() override;
        Expected<std::unique_ptr<Fence>, std::runtime_error> createFence(Fence::Descriptor descriptor) override;

        Expected<std::unique_ptr<GraphicsBuffer>, std::runtime_error> createBuffer(GraphicsBuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<GraphicsImage>, std::runtime_error> createImage(GraphicsImage::Descriptor descriptor) override;

        Expected<std::unique_ptr<Sampler>, std::runtime_error> createSampler(Sampler::Descriptor descriptor) override;

    private:
        /**
         * @brief Creates a Vulkan shader object from SPIR-V
         */
        Expected<std::unique_ptr<Shader>, std::runtime_error> createShaderObject(std::span<uint32_t> spirvSource);
    };
}