#include "Bulb/Rendering/RenderingHelpers.hpp"

#include "Bulb/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/Shader.hpp>
#include <Clove/Graphics/TransferQueue.hpp>
#include <Clove/Graphics/GraphicsQueue.hpp>

namespace blb::rnd {
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory &factory) {
        std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> setLayouts;

        //MESH SET
        {
            clv::gfx::DescriptorSetBindingInfo textureBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo skeletonBinding{
                .binding   = 1,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex,
            };

            setLayouts[DescriptorSetSlots::Mesh] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(textureBinding),
                    std::move(skeletonBinding),
                },
            });
        }

        //VIEW SET
        {
            clv::gfx::DescriptorSetBindingInfo viewDataBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex,
            };

            clv::gfx::DescriptorSetBindingInfo viewPosBinding{
                .binding   = 1,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::View] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(viewDataBinding),
                    std::move(viewPosBinding) },
            });
        }

        //LIGHTING SET
        {
            clv::gfx::DescriptorSetBindingInfo lightDataBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo numLightBinding{
                .binding   = 1,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex | clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo directionalShadowTransformBinding{
                .binding   = 2,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex,
            };

            clv::gfx::DescriptorSetBindingInfo directionalShadowMapBinding{
                .binding   = 3,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo pointShadowMapBinding{
                .binding   = 4,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::Lighting] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(lightDataBinding),
                    std::move(numLightBinding),
                    std::move(directionalShadowTransformBinding),
                    std::move(directionalShadowMapBinding),
                    std::move(pointShadowMapBinding),
                },
            });
        }

        return setLayouts;
    }

    std::unordered_map<clv::gfx::DescriptorType, uint32_t> countDescriptorBindingTypes(clv::gfx::DescriptorSetLayout const &descriptorSetLayout) {
        std::unordered_map<clv::gfx::DescriptorType, uint32_t> counts;
        auto const &descriptor = descriptorSetLayout.getDescriptor();
        for(auto &binding : descriptor.bindings) {
            counts[binding.type]++;
        }

        return counts;
    }

    std::unique_ptr<clv::gfx::GraphicsImage> createImageWithData(clv::gfx::GraphicsFactory &factory, clv::gfx::GraphicsImage::Descriptor imageDescriptor, void const *data, size_t const dataSize) {
        using namespace clv::gfx;

        ImageMemoryBarrierInfo constexpr layoutTransferInfo{
            .sourceAccess      = AccessFlags::None,
            .destinationAccess = AccessFlags::TransferWrite,
            .oldImageLayout    = GraphicsImage::Layout::Undefined,
            .newImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
            .sourceQueue       = QueueType::None,
            .destinationQueue  = QueueType::None,
        };

        ImageMemoryBarrierInfo constexpr transferQueueReleaseInfo{
            .sourceAccess      = AccessFlags::TransferWrite,
            .destinationAccess = AccessFlags::None,
            .oldImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
            .newImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
            .sourceQueue       = QueueType::Transfer,
            .destinationQueue  = QueueType::Graphics,
        };

        ImageMemoryBarrierInfo constexpr graphicsQueueAcquireInfo{
            .sourceAccess      = AccessFlags::TransferWrite,
            .destinationAccess = AccessFlags::ShaderRead,
            .oldImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
            .newImageLayout    = GraphicsImage::Layout::ShaderReadOnlyOptimal,
            .sourceQueue       = QueueType::Transfer,
            .destinationQueue  = QueueType::Graphics,
        };

        size_t constexpr bufferOffset{ 0 };
        clv::mth::vec3i constexpr imageOffset{ 0, 0, 0 };
        clv::mth::vec3ui const imageExtent{ imageDescriptor.dimensions.x, imageDescriptor.dimensions.y, 1 };

        auto transferQueue = factory.createTransferQueue({ QueueFlags::Transient });
        auto graphicsQueue = factory.createGraphicsQueue({ QueueFlags::Transient });

        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();
        std::shared_ptr<GraphicsCommandBuffer> graphicsCommandBuffer = graphicsQueue->allocateCommandBuffer();

        auto image = factory.createImage(std::move(imageDescriptor));

        auto transferBuffer = factory.createBuffer(GraphicsBuffer::Descriptor{
            .size        = dataSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferSource,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });
        transferBuffer->write(data, bufferOffset, dataSize);

        //Change the layout of the image, write the buffer into it and then release the queue ownership
        transferCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->imageMemoryBarrier(*image, std::move(layoutTransferInfo), PipelineObject::Stage::Top, PipelineObject::Stage::Transfer);
        transferCommandBuffer->copyBufferToImage(*transferBuffer, bufferOffset, *image, imageOffset, imageExtent);
        transferCommandBuffer->imageMemoryBarrier(*image, std::move(transferQueueReleaseInfo), PipelineObject::Stage::Transfer, PipelineObject::Stage::Transfer);
        transferCommandBuffer->endRecording();

        //Acquire ownership of the image to a graphics queue
        graphicsCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        graphicsCommandBuffer->imageMemoryBarrier(*image, std::move(graphicsQueueAcquireInfo), PipelineObject::Stage::Transfer, PipelineObject::Stage::PixelShader);
        graphicsCommandBuffer->endRecording();

        auto graphicsQueueFinishedFence = factory.createFence({ false });

        transferQueue->submit(TransferSubmitInfo{ .commandBuffers = { transferCommandBuffer } });
        graphicsQueue->submit(GraphicsSubmitInfo{ .commandBuffers = { graphicsCommandBuffer } }, graphicsQueueFinishedFence.get());

        transferQueue->freeCommandBuffer(*transferCommandBuffer);
        graphicsQueueFinishedFence->wait();
        graphicsQueue->freeCommandBuffer(*graphicsCommandBuffer);

        return image;
    }
}