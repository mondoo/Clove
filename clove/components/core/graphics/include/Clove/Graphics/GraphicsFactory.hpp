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

#include <Clove/Expected.hpp>
#include <span>
#include <string>

namespace garlic::clove {
    /**
     * @brief Used to create graphics objects. Abstracts away the underlying API.
     */
    class GraphicsFactory {
        //FUNCTIONS
    public:
        virtual ~GraphicsFactory() = default;

        virtual Expected<std::unique_ptr<GraphicsQueue>, std::exception> createGraphicsQueue(CommandQueueDescriptor descriptor) = 0;
        virtual Expected<std::unique_ptr<PresentQueue>, std::exception> createPresentQueue()                                    = 0;
        virtual Expected<std::unique_ptr<TransferQueue>, std::exception> createTransferQueue(CommandQueueDescriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<Swapchain>, std::exception> createSwapChain(Swapchain::Descriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<Shader>, std::exception> createShader(std::string_view filePath)           = 0;
        virtual Expected<std::unique_ptr<Shader>, std::exception> createShader(std::span<std::byte const> byteCode) = 0;

        virtual Expected<std::unique_ptr<RenderPass>, std::exception> createRenderPass(RenderPass::Descriptor descriptor)                            = 0;
        virtual Expected<std::unique_ptr<DescriptorSetLayout>, std::exception> createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<PipelineObject>, std::exception> createPipelineObject(PipelineObject::Descriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<Framebuffer>, std::exception> createFramebuffer(Framebuffer::Descriptor descriptor)          = 0;
        virtual Expected<std::unique_ptr<DescriptorPool>, std::exception> createDescriptorPool(DescriptorPool::Descriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<Semaphore>, std::exception> createSemaphore()                     = 0;
        virtual Expected<std::unique_ptr<Fence>, std::exception> createFence(Fence::Descriptor descriptor) = 0;

        virtual Expected<std::unique_ptr<GraphicsBuffer>, std::exception> createBuffer(GraphicsBuffer::Descriptor descriptor) = 0;
        virtual Expected<std::unique_ptr<GraphicsImage>, std::exception> createImage(GraphicsImage::Descriptor descriptor)    = 0;

        virtual Expected<std::unique_ptr<Sampler>, std::exception> createSampler(Sampler::Descriptor descriptor) = 0;
    };
}