#include "Bulb/Rendering/Material.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

namespace blb::rnd {
    std::weak_ptr<clv::gfx::GraphicsImage> Material::defaultImage{};

    Material::Material(clv::gfx::GraphicsFactory &factory) {
        using namespace clv::gfx;

        if(defaultImage.use_count() == 0) {
            clv::mth::vec2f constexpr imageDimensions{ 1.0f, 1.0f };
            uint32_t constexpr bytesPerPixel{ 4 };
            uint32_t constexpr white{ 0xffffffff };

            auto transferQueue = factory.createTransferQueue({ QueueFlags::Transient });
            auto graphicsQueue = factory.createGraphicsQueue({ QueueFlags::Transient });

            std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();
            std::shared_ptr<GraphicsCommandBuffer> graphicsCommandBuffer = graphicsQueue->allocateCommandBuffer();

            std::shared_ptr<GraphicsImage> image = factory.createImage(GraphicsImage::Descriptor{
                .type        = GraphicsImage::Type::_2D,
                .usageFlags  = GraphicsImage::UsageMode::TransferDestination | GraphicsImage::UsageMode::Sampled,
                .dimensions  = imageDimensions,
                .format      = GraphicsImage::Format::R8G8B8A8_SRGB,
                .sharingMode = SharingMode::Exclusive,
                .memoryType  = MemoryType::VideoMemory,
            });

            auto transferBuffer = factory.createBuffer(GraphicsBuffer::Descriptor{
                .size        = bytesPerPixel,
                .usageFlags  = GraphicsBuffer::UsageMode::TransferSource,
                .sharingMode = SharingMode::Exclusive,
                .memoryType  = MemoryType::SystemMemory,
            });
            transferBuffer->write(&white, 0, bytesPerPixel);

            //Change the layout of the default image, write the buffer into it and then release the queue ownership
            ImageMemoryBarrierInfo const layoutTransferInfo{
                .sourceAccess      = AccessFlags::None,
                .destinationAccess = AccessFlags::TransferWrite,
                .oldImageLayout    = GraphicsImage::Layout::Undefined,
                .newImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
                .sourceQueue       = QueueType::None,
                .destinationQueue  = QueueType::None,
            };

            ImageMemoryBarrierInfo const transferQueueReleaseInfo{
                .sourceAccess      = AccessFlags::TransferWrite,
                .destinationAccess = AccessFlags::None,
                .oldImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
                .newImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
                .sourceQueue       = QueueType::Transfer,
                .destinationQueue  = QueueType::Graphics,
            };

            transferCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
            transferCommandBuffer->imageMemoryBarrier(*image, std::move(layoutTransferInfo), PipelineObject::Stage::Top, PipelineObject::Stage::Transfer);
            transferCommandBuffer->copyBufferToImage(*transferBuffer, 0, *image, GraphicsImage::Layout::TransferDestinationOptimal, { 0, 0, 0 }, { imageDimensions.x, imageDimensions.y, 1 });
            transferCommandBuffer->imageMemoryBarrier(*image, std::move(transferQueueReleaseInfo), PipelineObject::Stage::Transfer, PipelineObject::Stage::Transfer);
            transferCommandBuffer->endRecording();

            //Acquire ownership of the image to a graphics queue
            ImageMemoryBarrierInfo const graphicsQueueAcquireInfo{
                .sourceAccess      = AccessFlags::TransferWrite,
                .destinationAccess = AccessFlags::ShaderRead,
                .oldImageLayout    = GraphicsImage::Layout::TransferDestinationOptimal,
                .newImageLayout    = GraphicsImage::Layout::ShaderReadOnlyOptimal,
                .sourceQueue       = QueueType::Transfer,
                .destinationQueue  = QueueType::Graphics,
            };

            graphicsCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
            graphicsCommandBuffer->imageMemoryBarrier(*image, std::move(graphicsQueueAcquireInfo), PipelineObject::Stage::Transfer, PipelineObject::Stage::PixelShader);
            graphicsCommandBuffer->endRecording();

            auto graphicsQueueFinishedFence = factory.createFence({ false });

            transferQueue->submit(TransferSubmitInfo{ .commandBuffers = { transferCommandBuffer } });
            graphicsQueue->submit(GraphicsSubmitInfo{ .commandBuffers = { graphicsCommandBuffer } }, graphicsQueueFinishedFence.get());

            transferQueue->freeCommandBuffer(*transferCommandBuffer);
            graphicsQueueFinishedFence->wait();
            graphicsQueue->freeCommandBuffer(*graphicsCommandBuffer);

            defaultImage = image;

            diffuseImage  = image;
            specularImage = image;
        } else {
            diffuseImage  = defaultImage.lock();
            specularImage = defaultImage.lock();
        }

        GraphicsImageView::Descriptor const viewDescriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        };

        diffuseView  = diffuseImage->createView(viewDescriptor);
        specularView = specularImage->createView(std::move(viewDescriptor));
    }

    Material::Material(Material const &other) = default;

    Material::Material(Material &&other) noexcept = default;

    Material &Material::operator=(Material const &other) = default;

    Material &Material::operator=(Material &&other) noexcept = default;

    Material::~Material() = default;

    void Material::setDiffuseTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        diffuseImage = std::move(image);
        diffuseView  = diffuseImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    void Material::setSpecularTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        specularImage = std::move(image);
        specularView  = specularImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }
}