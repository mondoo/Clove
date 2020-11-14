#include "Clove/Graphics/Vulkan/VKGraphicsFactory.hpp"

#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKPresentQueue.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKSampler.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"
#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

namespace clv::gfx::vk {
    VKGraphicsFactory::VKGraphicsFactory(DevicePointer devicePtr, QueueFamilyIndices queueFamilyIndices, SwapchainSupportDetails swapchainSupportDetails)
        : devicePtr(std::move(devicePtr))
        , queueFamilyIndices(std::move(queueFamilyIndices))
        , swapchainSupportDetails(std::move(swapchainSupportDetails)) {
        memoryAllocator = std::make_shared<MemoryAllocator>(this->devicePtr);
    }

    VKGraphicsFactory::VKGraphicsFactory(VKGraphicsFactory&& other) noexcept = default;

    VKGraphicsFactory& VKGraphicsFactory::operator=(VKGraphicsFactory&& other) noexcept = default;

    VKGraphicsFactory::~VKGraphicsFactory() = default;

    std::unique_ptr<GraphicsQueue> VKGraphicsFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) {
        return std::make_unique<VKGraphicsQueue>(devicePtr, queueFamilyIndices, std::move(descriptor));
    }

    std::unique_ptr<PresentQueue> VKGraphicsFactory::createPresentQueue() {
        return std::make_unique<VKPresentQueue>(devicePtr, *queueFamilyIndices.presentFamily);
    }

    std::unique_ptr<TransferQueue> VKGraphicsFactory::createTransferQueue(CommandQueueDescriptor descriptor) {
        return std::make_unique<VKTransferQueue>(devicePtr, queueFamilyIndices, std::move(descriptor));
    }

    std::unique_ptr<Swapchain> VKGraphicsFactory::createSwapChain(Swapchain::Descriptor descriptor) {
        return std::make_unique<VKSwapchain>(devicePtr, swapchainSupportDetails, queueFamilyIndices, std::move(descriptor));
    }

    std::unique_ptr<Shader> VKGraphicsFactory::createShader(std::string_view filePath) {
        return std::make_unique<VKShader>(devicePtr, filePath);
    }

    std::unique_ptr<Shader> VKGraphicsFactory::createShader(std::span<const std::byte> byteCode) {
        return std::make_unique<VKShader>(devicePtr, std::move(byteCode));
    }

    std::unique_ptr<RenderPass> VKGraphicsFactory::createRenderPass(RenderPass::Descriptor descriptor) {
        return std::make_unique<VKRenderPass>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<DescriptorSetLayout> VKGraphicsFactory::createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) {
        return std::make_unique<VKDescriptorSetLayout>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<PipelineObject> VKGraphicsFactory::createPipelineObject(PipelineObject::Descriptor descriptor) {
        return std::make_unique<VKPipelineObject>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<Framebuffer> VKGraphicsFactory::createFramebuffer(Framebuffer::Descriptor descriptor) {
        return std::make_unique<VKFramebuffer>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<DescriptorPool> VKGraphicsFactory::createDescriptorPool(DescriptorPool::Descriptor descriptor) {
        return std::make_unique<VKDescriptorPool>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<Semaphore> VKGraphicsFactory::createSemaphore() {
        return std::make_unique<VKSemaphore>(devicePtr);
    }

    std::unique_ptr<Fence> VKGraphicsFactory::createFence(Fence::Descriptor descriptor) {
        return std::make_unique<VKFence>(devicePtr, std::move(descriptor));
    }

    std::unique_ptr<GraphicsBuffer> VKGraphicsFactory::createBuffer(GraphicsBuffer::Descriptor descriptor) {
        return std::make_unique<VKBuffer>(devicePtr, std::move(descriptor), queueFamilyIndices, memoryAllocator);
    }

    std::unique_ptr<GraphicsImage> VKGraphicsFactory::createImage(GraphicsImage::Descriptor descriptor) {
        return std::make_unique<VKImage>(devicePtr, std::move(descriptor), queueFamilyIndices, memoryAllocator);
    }

    std::unique_ptr<Sampler> VKGraphicsFactory::createSampler(Sampler::Descriptor descriptor) {
        return std::make_unique<VKSampler>(devicePtr, std::move(descriptor));
    }
}