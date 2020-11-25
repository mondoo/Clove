#include "Clove/Rendering/RenderingHelpers.hpp"

#include "Clove/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsQueue.hpp>
#include <Clove/Graphics/Shader.hpp>
#include <Clove/Graphics/TransferQueue.hpp>

using namespace garlic::clove;

namespace garlic::clove {
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<DescriptorSetLayout>> createDescriptorSetLayouts(GraphicsFactory &factory) {
        std::unordered_map<DescriptorSetSlots, std::shared_ptr<DescriptorSetLayout>> setLayouts;

        //MESH SET
        {
            DescriptorSetBindingInfo textureBinding{
                .binding   = 0,
                .type      = DescriptorType::CombinedImageSampler,
                .arraySize = 1,
                .stage     = Shader::Stage::Pixel,
            };

            DescriptorSetBindingInfo modelBinding{
                .binding   = 1,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Vertex,
            };

            DescriptorSetBindingInfo skeletonBinding{
                .binding   = 2,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Vertex,
            };

            DescriptorSetBindingInfo colourBinding{
                .binding   = 3,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::Mesh] = factory.createDescriptorSetLayout(DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(textureBinding),
                    std::move(modelBinding),
                    std::move(skeletonBinding),
                    std::move(colourBinding),
                },
            });
        }

        //VIEW SET
        {
            DescriptorSetBindingInfo viewDataBinding{
                .binding   = 0,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Vertex,
            };

            DescriptorSetBindingInfo viewPosBinding{
                .binding   = 1,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::View] = factory.createDescriptorSetLayout(DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(viewDataBinding),
                    std::move(viewPosBinding),
                },
            });
        }

        //LIGHTING SET
        {
            DescriptorSetBindingInfo lightDataBinding{
                .binding   = 0,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Pixel,
            };

            DescriptorSetBindingInfo numLightBinding{
                .binding   = 1,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Vertex | Shader::Stage::Pixel,
            };

            DescriptorSetBindingInfo directionalShadowTransformBinding{
                .binding   = 2,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = Shader::Stage::Vertex,
            };

            DescriptorSetBindingInfo directionalShadowMapBinding{
                .binding   = 3,
                .type      = DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = Shader::Stage::Pixel,
            };

            DescriptorSetBindingInfo pointShadowMapBinding{
                .binding   = 4,
                .type      = DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::Lighting] = factory.createDescriptorSetLayout(DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(lightDataBinding),
                    std::move(numLightBinding),
                    std::move(directionalShadowTransformBinding),
                    std::move(directionalShadowMapBinding),
                    std::move(pointShadowMapBinding),
                },
            });
        }

        //UI SET
        {
            DescriptorSetBindingInfo textureBinding{
                .binding   = 0,
                .type      = DescriptorType::CombinedImageSampler,
                .arraySize = 1,
                .stage     = Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::UI] = factory.createDescriptorSetLayout(DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(textureBinding),
                },
            });
        }

        return setLayouts;
    }

    std::unordered_map<DescriptorType, uint32_t> countDescriptorBindingTypes(DescriptorSetLayout const &descriptorSetLayout) {
        std::unordered_map<DescriptorType, uint32_t> counts;
        auto const &descriptor = descriptorSetLayout.getDescriptor();
        for(auto &binding : descriptor.bindings) {
            counts[binding.type] += binding.arraySize;
        }

        return counts;
    }

    std::unique_ptr<GraphicsImage> createImageWithData(GraphicsFactory &factory, GraphicsImage::Descriptor imageDescriptor, void const *data, size_t const dataSize) {
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
        vec3i constexpr imageOffset{ 0, 0, 0 };
        vec3ui const imageExtent{ imageDescriptor.dimensions.x, imageDescriptor.dimensions.y, 1 };

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

        auto transferQueueFinishedFence = factory.createFence({ false });
        auto graphicsQueueFinishedFence = factory.createFence({ false });

        transferQueue->submit({ TransferSubmitInfo{ .commandBuffers = { transferCommandBuffer } } }, transferQueueFinishedFence.get());
        graphicsQueue->submit({ GraphicsSubmitInfo{ .commandBuffers = { graphicsCommandBuffer } } }, graphicsQueueFinishedFence.get());

        transferQueueFinishedFence->wait();
        transferQueue->freeCommandBuffer(*transferCommandBuffer);
        
        graphicsQueueFinishedFence->wait();
        graphicsQueue->freeCommandBuffer(*graphicsCommandBuffer);

        return image;
    }
}