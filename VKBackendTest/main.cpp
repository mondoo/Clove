#include <Bulb/Bulb.hpp>

#include <Bulb/Rendering/ForwardRenderer3D.hpp>
#include <Bulb/Rendering/Vertex.hpp>
#include <Bulb/TextureLoader.hpp>
#include <Clove/Graphics/GraphicsTypes.hpp>
#include <Bulb/Rendering/Material.hpp>
#include <Bulb/Rendering/Renderables/Mesh.hpp>

int main(){
	auto platform = clv::plt::createPlatformInstance();
	auto mainWindow = platform->createWindow({ "Vulkan refactor!", 1280, 720 });
	mainWindow->setVSync(true);

	auto* renderer = new blb::rnd::ForwardRenderer3D(*mainWindow, clv::gfx::API::Vulkan);
    auto graphicsFactory = renderer->getGraphicsFactory();

    auto graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::None });
    auto transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

    const std::vector<blb::rnd::Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

        { { -0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    };
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0, 
        4, 5, 6, 6, 7, 4
    };

    std::shared_ptr<clv::gfx::GraphicsImage> texture;
    std::shared_ptr<clv::gfx::GraphicsImageView> imageView;

    //CREATE THE THINGS
    const auto textureData = blb::TextureLoader::loadTexture("F:/RingsOfPower/Engine/Garlic/VKBackendTest/texture.jpg");

    const size_t textureBufferSize = textureData.dimensions.x * textureData.dimensions.y * textureData.channels;

    //Staging buffer
    clv::gfx::GraphicsBuffer::Descriptor stagingDescriptor{};
    stagingDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferSource;
    stagingDescriptor.sharingMode = clv::gfx::SharingMode::Exclusive;//Only accessed by the transfer queue
    stagingDescriptor.memoryType  = clv::gfx::MemoryType::SystemMemory;
    stagingDescriptor.size    = textureBufferSize;
    auto stagingBufferTexture = graphicsFactory->createBuffer(stagingDescriptor);

    //Texture Image
    clv::gfx::GraphicsImage::Descriptor textureDescriptor{};
    textureDescriptor.type        = clv::gfx::GraphicsImage::Type::_2D;
    textureDescriptor.usageFlags  = clv::gfx::GraphicsImage::UsageMode::TransferDestination | clv::gfx::GraphicsImage::UsageMode::Sampled;
    textureDescriptor.dimensions  = textureData.dimensions;
    textureDescriptor.format      = clv::gfx::ImageFormat::R8G8B8A8_SRGB;//This will be how the texels in the staging buffer will be layed out (when they are mapped from the raw buffer)
    textureDescriptor.sharingMode = clv::gfx::SharingMode::Exclusive;
    textureDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;//We have no way to write to images directly, but we still only plan on pushing data once

    texture = graphicsFactory->createImage(textureDescriptor);

    //Map the data onto our CPU visible (staging) buffers
    stagingBufferTexture->map(textureData.buffer.get(), textureBufferSize);

    //Transfer the data onto our GPU optimised buffers
    std::shared_ptr<clv::gfx::TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

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
    transferCommandBuffer->imageMemoryBarrier(*texture, layoutTransferInfo, clv::gfx::PipelineStage::Top, clv::gfx::PipelineStage::Transfer);
    transferCommandBuffer->copyBufferToImage(*stagingBufferTexture, 0, *texture, clv::gfx::ImageLayout::TransferDestinationOptimal, { 0, 0, 0 }, { textureData.dimensions.x, textureData.dimensions.y, 1 });
    transferCommandBuffer->imageMemoryBarrier(*texture, transferQueueReleaseInfo, clv::gfx::PipelineStage::Transfer, clv::gfx::PipelineStage::Transfer);
    transferCommandBuffer->endRecording();

    //Transitioning an image layout to shader optimal can only be done on a command buffer that's part of the graphics queue family
    std::shared_ptr<clv::gfx::GraphicsCommandBuffer> transitionCommandBuffer = graphicsQueue->allocateCommandBuffer();

    clv::gfx::ImageMemoryBarrierInfo graphicsQueueAcquireInfo{};
    graphicsQueueAcquireInfo.sourceAccess      = clv::gfx::AccessFlags::TransferWrite;
    graphicsQueueAcquireInfo.destinationAccess = clv::gfx::AccessFlags::ShaderRead;
    graphicsQueueAcquireInfo.oldImageLayout    = clv::gfx::ImageLayout::TransferDestinationOptimal;
    graphicsQueueAcquireInfo.newImageLayout    = clv::gfx::ImageLayout::ShaderReadOnlyOptimal;
    graphicsQueueAcquireInfo.sourceQueue       = clv::gfx::QueueType::Transfer;
    graphicsQueueAcquireInfo.destinationQueue  = clv::gfx::QueueType::Graphics;

    transitionCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
    transitionCommandBuffer->imageMemoryBarrier(*texture, graphicsQueueAcquireInfo, clv::gfx::PipelineStage::Transfer, clv::gfx::PipelineStage::PixelShader);
    transitionCommandBuffer->endRecording();

    transferQueue->submit({ { transferCommandBuffer } });
    transferQueue->freeCommandBuffer(*transferCommandBuffer);

    auto transferCompleteFance = graphicsFactory->createFence({ false });

    clv::gfx::GraphicsSubmitInfo submitInfo{};
    submitInfo.commandBuffers = { { transitionCommandBuffer } };
    graphicsQueue->submit(submitInfo, transferCompleteFance.get());
    transferCompleteFance->wait();
    graphicsQueue->freeCommandBuffer(*transitionCommandBuffer);

    blb::rnd::Material material(*graphicsFactory);
    material.setDiffuseTexture(std::move(texture));
    std::shared_ptr<blb::rnd::Mesh> mesh = std::make_shared<blb::rnd::Mesh>(vertices, indices, std::move(material), *graphicsFactory);

	while(mainWindow->isOpen()) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		mainWindow->processInput();
		if(mainWindow->getKeyboard().isKeyPressed(clv::Key::Escape)) {
			break;
		}

		renderer->begin();

        const auto transform = clv::mth::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        const auto position  = clv::mth::translate(clv::mth::mat4f{ 1.0f }, { 0.0f, 0.0f, 2.0f });
        renderer->submitStaticMesh(mesh, transform);
        renderer->submitStaticMesh(mesh, position);

		renderer->end();
	}

    //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
    delete renderer;

    texture.reset();
}