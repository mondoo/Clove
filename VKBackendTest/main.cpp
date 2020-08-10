#include <Bulb/Bulb.hpp>

#include <Bulb/Rendering/ForwardRenderer3D.hpp>
#include <Bulb/Rendering/RenderingTypes.hpp>
#include <Bulb/TextureLoader.hpp>

int main(){
	auto platform = clv::plt::createPlatformInstance();
	auto mainWindow = platform->createWindow({ "Vulkan refactor!", 1280, 720 });
	mainWindow->setVSync(true);

	auto renderer = blb::rnd::ForwardRenderer3D(*mainWindow, clv::gfx::API::Vulkan);
    auto graphicsFactory = renderer.getGraphicsFactory();

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
    std::shared_ptr<clv::gfx::GraphicsBuffer> vertexBuffer;
    std::shared_ptr<clv::gfx::GraphicsBuffer> indexBuffer;
    std::shared_ptr<clv::gfx::GraphicsImage> texture;
    std::shared_ptr<clv::gfx::GraphicsImageView> imageView;
    std::shared_ptr<clv::gfx::Sampler> sampler;


    //CREATE THE THINGS
    const auto textureData = blb::TextureLoader::loadTexture("F:/RingsOfPower/Engine/Garlic/VKBackendTest/texture.jpg");

    const size_t vertexBufferSize  = sizeof(blb::rnd::Vertex) * std::size(vertices);
    const size_t indexBufferSize   = sizeof(uint16_t) * std::size(indices);
    const size_t textureBufferSize = textureData.dimensions.x * textureData.dimensions.y * textureData.channels;

    //Staging buffers
    clv::gfx::GraphicsBuffer::Descriptor stagingDescriptor{};
    stagingDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferSource;
    stagingDescriptor.sharingMode = clv::gfx::SharingMode::Exclusive;//Only accessed by the transfer queue
    stagingDescriptor.memoryType  = clv::gfx::MemoryType::SystemMemory;

    stagingDescriptor.size   = vertexBufferSize;
    auto stagingBufferVertex = graphicsFactory->createBuffer(stagingDescriptor);

    stagingDescriptor.size  = indexBufferSize;
    auto stagingBufferIndex = graphicsFactory->createBuffer(stagingDescriptor);

    stagingDescriptor.size    = textureBufferSize;
    auto stagingBufferTexture = graphicsFactory->createBuffer(stagingDescriptor);

    //Vertex buffer
    clv::gfx::GraphicsBuffer::Descriptor vertexDescriptor{};
    vertexDescriptor.size        = vertexBufferSize;
    vertexDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferDestination | clv::gfx::GraphicsBuffer::UsageMode::VertexBuffer;
    vertexDescriptor.sharingMode = clv::gfx::SharingMode::Concurrent;//Accessed by transfer and graphics queue
    vertexDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

    vertexBuffer = graphicsFactory->createBuffer(vertexDescriptor);

    //Index Buffer
    clv::gfx::GraphicsBuffer::Descriptor indexDescriptor{};
    indexDescriptor.size        = indexBufferSize;
    indexDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferDestination | clv::gfx::GraphicsBuffer::UsageMode::IndexBuffer;
    indexDescriptor.sharingMode = clv::gfx::SharingMode::Concurrent;
    indexDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

    indexBuffer = graphicsFactory->createBuffer(indexDescriptor);

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
    stagingBufferVertex->map(std::data(vertices), vertexBufferSize);
    stagingBufferIndex->map(std::data(indices), indexBufferSize);
    stagingBufferTexture->map(textureData.buffer.get(), textureBufferSize);

    //Transfer the data onto our GPU optimised buffers
    std::shared_ptr<clv::gfx::TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

    transferCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
    transferCommandBuffer->copyBufferToBuffer(*stagingBufferVertex, 0, *vertexBuffer, 0, vertexBufferSize);
    transferCommandBuffer->copyBufferToBuffer(*stagingBufferIndex, 0, *indexBuffer, 0, indexBufferSize);
    transferCommandBuffer->transitionImageLayout(*texture, clv::gfx::ImageLayout::Undefined, clv::gfx::ImageLayout::TransferDestinationOptimal);
    transferCommandBuffer->copyBufferToImage(*stagingBufferTexture, 0, *texture, clv::gfx::ImageLayout::TransferDestinationOptimal, { 0, 0, 0 }, { textureData.dimensions.x, textureData.dimensions.y, 1 });
    transferCommandBuffer->endRecording();

    //Transitioning an image layout to shader optimal can only be done on a command buffer that's part of the graphics queue family
    std::shared_ptr<clv::gfx::GraphicsCommandBuffer> transitionCommandBuffer = graphicsQueue->allocateCommandBuffer();

    transitionCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
    transitionCommandBuffer->transitionImageLayout(*texture, clv::gfx::ImageLayout::TransferDestinationOptimal, clv::gfx::ImageLayout::ShaderReadOnlyOptimal);
    transitionCommandBuffer->endRecording();

    transferQueue->submit({ { transferCommandBuffer } });
    transferQueue->freeCommandBuffer(*transferCommandBuffer);

    clv::gfx::GraphicsSubmitInfo submitInfo{};
    submitInfo.commandBuffers = { { transitionCommandBuffer } };
    graphicsQueue->submit(submitInfo, nullptr);
    graphicsFactory->waitForIdleDevice();//We need to wait for out buffer to be executed before freeing
    graphicsQueue->freeCommandBuffer(*transitionCommandBuffer);

    //Create the objects we need to send the image to the shaders
    clv::gfx::Sampler::Descriptor samplerDescriptor{};
    samplerDescriptor.minFilter        = clv::gfx::Sampler::Filter::Linear;
    samplerDescriptor.magFilter        = clv::gfx::Sampler::Filter::Linear;
    samplerDescriptor.addressModeU     = clv::gfx::Sampler::AddressMode::Repeat;
    samplerDescriptor.addressModeV     = clv::gfx::Sampler::AddressMode::Repeat;
    samplerDescriptor.addressModeW     = clv::gfx::Sampler::AddressMode::Repeat;
    samplerDescriptor.enableAnisotropy = true;
    samplerDescriptor.maxAnisotropy    = 16.0f;

    imageView = texture->createView();
    sampler   = graphicsFactory->createSampler(std::move(samplerDescriptor));

	while(mainWindow->isOpen()) {
		mainWindow->processInput();
		if(mainWindow->getKeyboard().isKeyPressed(clv::Key::Escape)) {
			break;
		}

		renderer.begin();

		//TODO: Submit mesh data

		renderer.end();
	}

    //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
    vertexBuffer.reset();
    indexBuffer.reset();
    uniformBuffers.clear();
    texture.reset();
    sampler.reset();
    imageView.reset();
}