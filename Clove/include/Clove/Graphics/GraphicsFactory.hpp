#pragma once

#include "Clove/Graphics/DescriptorPool.hpp"
#include "Clove/Graphics/DescriptorSetLayout.hpp"
#include "Clove/Graphics/Fence.hpp"
#include "Clove/Graphics/Framebuffer.hpp"
#include "Clove/Graphics/GraphicsBuffer.hpp"
#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/GraphicsQueue.hpp"
#include "Clove/Graphics/PipelineObject.hpp"
#include "Clove/Graphics/PresentQueue.hpp"
#include "Clove/Graphics/RenderPass.hpp"
#include "Clove/Graphics/Sampler.hpp"
#include "Clove/Graphics/Semaphore.hpp"
#include "Clove/Graphics/Shader.hpp"
#include "Clove/Graphics/Swapchain.hpp"
#include "Clove/Graphics/TransferQueue.hpp"

namespace clv::gfx {
    /**
     * @brief Used to create graphics objects. Abstracts away the underlying API.
     */
    class GraphicsFactory {
        //FUNCTIONS
    public:
        virtual ~GraphicsFactory() = default;

        virtual std::unique_ptr<GraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor) = 0;
        virtual std::unique_ptr<PresentQueue> createPresentQueue()                                    = 0;
        virtual std::unique_ptr<TransferQueue> createTransferQueue(CommandQueueDescriptor descriptor) = 0;

        virtual std::unique_ptr<Swapchain> createSwapChain(Swapchain::Descriptor descriptor) = 0;

        virtual std::unique_ptr<Shader> createShader(std::string_view filePath)       = 0;
        virtual std::unique_ptr<Shader> createShader(std::vector<std::byte> byteCode) = 0;
        virtual std::unique_ptr<Shader> createShader(const std::byte* byteCode, const size_t numBytes) = 0;

        virtual std::unique_ptr<RenderPass> createRenderPass(RenderPass::Descriptor descriptor)                            = 0;
        virtual std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) = 0;

        virtual std::unique_ptr<PipelineObject> createPipelineObject(PipelineObject::Descriptor descriptor) = 0;

        virtual std::unique_ptr<Framebuffer> createFramebuffer(Framebuffer::Descriptor descriptor)          = 0;
        virtual std::unique_ptr<DescriptorPool> createDescriptorPool(DescriptorPool::Descriptor descriptor) = 0;

        virtual std::unique_ptr<Semaphore> createSemaphore()                     = 0;
        virtual std::unique_ptr<Fence> createFence(Fence::Descriptor descriptor) = 0;

        virtual std::unique_ptr<GraphicsBuffer> createBuffer(GraphicsBuffer::Descriptor descriptor) = 0;
        virtual std::unique_ptr<GraphicsImage> createImage(GraphicsImage::Descriptor descriptor)    = 0;

        virtual std::unique_ptr<Sampler> createSampler(Sampler::Descriptor descriptor) = 0;
    };
}