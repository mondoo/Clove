#include "Bulb/Rendering/Material.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

namespace blb::rnd {
    Material::Material(const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory) {
        using namespace clv::gfx;

        auto transferQueue = graphicsFactory->createTransferQueue({ QueueFlags::Transient });
        auto graphicsQueue = graphicsFactory->createGraphicsQueue({ QueueFlags::Transient });

        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();
        std::shared_ptr<GraphicsCommandBuffer> graphicsCommandBuffer = graphicsQueue->allocateCommandBuffer();

        GraphicsImage::Descriptor imageDesc{};
        imageDesc.type        = GraphicsImage::Type::_2D;
        imageDesc.dimensions  = { 1, 1 };
        imageDesc.usageFlags  = GraphicsImage::UsageMode::TransferDestination | GraphicsImage::UsageMode::Sampled;
        imageDesc.format      = ImageFormat::R8G8B8A8_SRGB;
        imageDesc.sharingMode = SharingMode::Exclusive;
        imageDesc.memoryType  = MemoryType::VideoMemory;
        defaultImage          = graphicsFactory->createImage(imageDesc);

        GraphicsBuffer::Descriptor bufferDesc{};
        bufferDesc.size        = 4;
        bufferDesc.usageFlags  = GraphicsBuffer::UsageMode::TransferSource;
        bufferDesc.sharingMode = SharingMode::Exclusive;
        bufferDesc.memoryType  = MemoryType::SystemMemory;
        auto transferBuffer    = graphicsFactory->createBuffer(std::move(bufferDesc));

        uint32_t white = 0xffffffff;
        transferBuffer->map(&white, 4);

        //Change the layout of the default image, write the buffer into it and then release the queue ownership
        clv::gfx::ImageMemoryBarrierInfo layoutTransferInfo{};
        layoutTransferInfo.sourceAccess      = clv::gfx::AccessFlags::None;
        layoutTransferInfo.destinationAccess = clv::gfx::AccessFlags::TransferWrite;
        layoutTransferInfo.oldImageLayout    = clv::gfx::ImageLayout::Undefined;
        layoutTransferInfo.newImageLayout    = clv::gfx::ImageLayout::TransferDestinationOptimal;
        layoutTransferInfo.sourceQueue       = clv::gfx::QueueType::None;
        layoutTransferInfo.destinationQueue  = clv::gfx::QueueType::None;

        clv::gfx::ImageMemoryBarrierInfo transferQueueReleaseInfo{};
        transferQueueReleaseInfo.sourceAccess      = clv::gfx::AccessFlags::TransferWrite;
        transferQueueReleaseInfo.destinationAccess = clv::gfx::AccessFlags::None;
        transferQueueReleaseInfo.oldImageLayout    = clv::gfx::ImageLayout::TransferDestinationOptimal;
        transferQueueReleaseInfo.newImageLayout    = clv::gfx::ImageLayout::TransferDestinationOptimal;
        transferQueueReleaseInfo.sourceQueue       = clv::gfx::QueueType::Transfer;
        transferQueueReleaseInfo.destinationQueue  = clv::gfx::QueueType::Graphics;

        transferCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->imageMemoryBarrier(*defaultImage, layoutTransferInfo, clv::gfx::PipelineStage::Top, clv::gfx::PipelineStage::Transfer);
        transferCommandBuffer->copyBufferToImage(*transferBuffer, 0, *defaultImage, clv::gfx::ImageLayout::TransferDestinationOptimal, { 0, 0, 0 }, { imageDesc.dimensions.x, imageDesc.dimensions.y, 1 });
        transferCommandBuffer->imageMemoryBarrier(*defaultImage, transferQueueReleaseInfo, clv::gfx::PipelineStage::Transfer, clv::gfx::PipelineStage::Transfer);
        transferCommandBuffer->endRecording();

        //Acquire queue ownership to a graphics queue
        clv::gfx::ImageMemoryBarrierInfo graphicsQueueAcquireInfo{};
        graphicsQueueAcquireInfo.sourceAccess      = clv::gfx::AccessFlags::TransferWrite;
        graphicsQueueAcquireInfo.destinationAccess = clv::gfx::AccessFlags::ShaderRead;
        graphicsQueueAcquireInfo.oldImageLayout    = clv::gfx::ImageLayout::TransferDestinationOptimal;
        graphicsQueueAcquireInfo.newImageLayout    = clv::gfx::ImageLayout::ShaderReadOnlyOptimal;
        graphicsQueueAcquireInfo.sourceQueue       = clv::gfx::QueueType::Transfer;
        graphicsQueueAcquireInfo.destinationQueue  = clv::gfx::QueueType::Graphics;

        graphicsCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
        graphicsCommandBuffer->imageMemoryBarrier(*defaultImage, graphicsQueueAcquireInfo, clv::gfx::PipelineStage::Transfer, clv::gfx::PipelineStage::PixelShader);
        graphicsCommandBuffer->endRecording();
        
        auto fence = graphicsFactory->createFence({ false });

        GraphicsSubmitInfo submitInfo{};
        submitInfo.commandBuffers = { graphicsCommandBuffer };

        transferQueue->submit({ { transferCommandBuffer } });
        graphicsQueue->submit(std::move(submitInfo), fence.get());

        transferQueue->freeCommandBuffer(*transferCommandBuffer);
        fence->wait();
        graphicsQueue->freeCommandBuffer(*graphicsCommandBuffer);

        diffuseView  = defaultImage->createView();
        specularView = defaultImage->createView();
    }

    Material::Material(const Material& other) = default;

    Material::Material(Material&& other) noexcept = default;

    Material& Material::operator=(const Material& other) = default;

    Material& Material::operator=(Material&& other) noexcept = default;

    Material::~Material() = default;

    void Material::setDiffuseTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        diffuseImage = std::move(image);
    }

    void Material::setSpecularTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        specularImage = std::move(image);
    }

    void Material::setColour(clv::mth::vec4f colour) {
        this->colour = std::move(colour);
    }

    void Material::setShininess(float shininess) {
        this->shininess = shininess;
    }
}