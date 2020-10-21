#include "Bulb/Rendering/Material.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

namespace blb::rnd {
    std::weak_ptr<clv::gfx::GraphicsImage> Material::defaultImage{};

    Material::Material(clv::gfx::GraphicsFactory& factory) {
        using namespace clv::gfx;

        if(defaultImage.use_count() == 0) {
            auto transferQueue = factory.createTransferQueue({ QueueFlags::Transient });
            auto graphicsQueue = factory.createGraphicsQueue({ QueueFlags::Transient });

            std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();
            std::shared_ptr<GraphicsCommandBuffer> graphicsCommandBuffer = graphicsQueue->allocateCommandBuffer();

            GraphicsImage::Descriptor imageDesc{};
            imageDesc.type                       = GraphicsImage::Type::_2D;
            imageDesc.dimensions                 = { 1, 1 };
            imageDesc.usageFlags                 = GraphicsImage::UsageMode::TransferDestination | GraphicsImage::UsageMode::Sampled;
            imageDesc.format                     = GraphicsImage::Format::R8G8B8A8_SRGB;
            imageDesc.sharingMode                = SharingMode::Exclusive;
            imageDesc.memoryType                 = MemoryType::VideoMemory;
            std::shared_ptr<GraphicsImage> image = factory.createImage(imageDesc);

            GraphicsBuffer::Descriptor bufferDesc{};
            bufferDesc.size        = 4;
            bufferDesc.usageFlags  = GraphicsBuffer::UsageMode::TransferSource;
            bufferDesc.sharingMode = SharingMode::Exclusive;
            bufferDesc.memoryType  = MemoryType::SystemMemory;
            auto transferBuffer    = factory.createBuffer(std::move(bufferDesc));

            uint32_t white = 0xffffffff;
            transferBuffer->write(&white, 0, 4);

            //Change the layout of the default image, write the buffer into it and then release the queue ownership
            clv::gfx::ImageMemoryBarrierInfo layoutTransferInfo{};
            layoutTransferInfo.sourceAccess      = clv::gfx::AccessFlags::None;
            layoutTransferInfo.destinationAccess = clv::gfx::AccessFlags::TransferWrite;
            layoutTransferInfo.oldImageLayout    = clv::gfx::GraphicsImage::Layout::Undefined;
            layoutTransferInfo.newImageLayout    = clv::gfx::GraphicsImage::Layout::TransferDestinationOptimal;
            layoutTransferInfo.sourceQueue       = clv::gfx::QueueType::None;
            layoutTransferInfo.destinationQueue  = clv::gfx::QueueType::None;

            clv::gfx::ImageMemoryBarrierInfo transferQueueReleaseInfo{};
            transferQueueReleaseInfo.sourceAccess      = clv::gfx::AccessFlags::TransferWrite;
            transferQueueReleaseInfo.destinationAccess = clv::gfx::AccessFlags::None;
            transferQueueReleaseInfo.oldImageLayout    = clv::gfx::GraphicsImage::Layout::TransferDestinationOptimal;
            transferQueueReleaseInfo.newImageLayout    = clv::gfx::GraphicsImage::Layout::TransferDestinationOptimal;
            transferQueueReleaseInfo.sourceQueue       = clv::gfx::QueueType::Transfer;
            transferQueueReleaseInfo.destinationQueue  = clv::gfx::QueueType::Graphics;

            transferCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
            transferCommandBuffer->imageMemoryBarrier(*image, layoutTransferInfo, clv::gfx::PipelineObject::Stage::Top, clv::gfx::PipelineObject::Stage::Transfer);
            transferCommandBuffer->copyBufferToImage(*transferBuffer, 0, *image, clv::gfx::GraphicsImage::Layout::TransferDestinationOptimal, { 0, 0, 0 }, { imageDesc.dimensions.x, imageDesc.dimensions.y, 1 });
            transferCommandBuffer->imageMemoryBarrier(*image, transferQueueReleaseInfo, clv::gfx::PipelineObject::Stage::Transfer, clv::gfx::PipelineObject::Stage::Transfer);
            transferCommandBuffer->endRecording();

            //Acquire queue ownership to a graphics queue
            clv::gfx::ImageMemoryBarrierInfo graphicsQueueAcquireInfo{};
            graphicsQueueAcquireInfo.sourceAccess      = clv::gfx::AccessFlags::TransferWrite;
            graphicsQueueAcquireInfo.destinationAccess = clv::gfx::AccessFlags::ShaderRead;
            graphicsQueueAcquireInfo.oldImageLayout    = clv::gfx::GraphicsImage::Layout::TransferDestinationOptimal;
            graphicsQueueAcquireInfo.newImageLayout    = clv::gfx::GraphicsImage::Layout::ShaderReadOnlyOptimal;
            graphicsQueueAcquireInfo.sourceQueue       = clv::gfx::QueueType::Transfer;
            graphicsQueueAcquireInfo.destinationQueue  = clv::gfx::QueueType::Graphics;

            graphicsCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
            graphicsCommandBuffer->imageMemoryBarrier(*image, graphicsQueueAcquireInfo, clv::gfx::PipelineObject::Stage::Transfer, clv::gfx::PipelineObject::Stage::PixelShader);
            graphicsCommandBuffer->endRecording();

            auto fence = factory.createFence({ false });

            GraphicsSubmitInfo submitInfo{};
            submitInfo.commandBuffers = { graphicsCommandBuffer };

            transferQueue->submit({ { transferCommandBuffer } });
            graphicsQueue->submit(std::move(submitInfo), fence.get());

            transferQueue->freeCommandBuffer(*transferCommandBuffer);
            fence->wait();
            graphicsQueue->freeCommandBuffer(*graphicsCommandBuffer);

            defaultImage = image;

            diffuseImage  = image;
            specularImage = image;
        } else {
            diffuseImage  = defaultImage.lock();
            specularImage = defaultImage.lock();
        }

        clv::gfx::GraphicsImageView::Descriptor viewDescriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        };

        diffuseView  = diffuseImage->createView(viewDescriptor);
        specularView = specularImage->createView(std::move(viewDescriptor));
    }

    Material::Material(const Material& other) = default;

    Material::Material(Material&& other) noexcept = default;

    Material& Material::operator=(const Material& other) = default;

    Material& Material::operator=(Material&& other) noexcept = default;

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

    void Material::setColour(clv::mth::vec4f colour) {
        this->colour = std::move(colour);
    }

    void Material::setShininess(float shininess) {
        this->shininess = shininess;
    }
}