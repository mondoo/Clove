#include "Clove/Rendering/Renderables/Mesh.hpp"

#include "Clove/Application.hpp"

#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/GhaFactory.hpp>

namespace garlic::clove {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices)
        : vertices(std::move(vertices))
        , indices(std::move(indices)) {
        using namespace garlic::clove;

        GhaFactory &factory{ *Application::get().getGraphicsDevice()->getGraphicsFactory() };

        vertexBufferSize = sizeof(Vertex) * std::size(this->vertices);
        size_t const indexBufferSize{ sizeof(uint16_t) * std::size(this->indices) };
        size_t const totalSize{ vertexBufferSize + indexBufferSize };

        vertexOffset = 0;
        indexOffset  = vertexBufferSize;

        auto transferQueue{ *factory.createTransferQueue({ QueueFlags::Transient }) };

        std::shared_ptr<GhaTransferCommandBuffer> transferCommandBuffer{ transferQueue->allocateCommandBuffer() };

        //Staging buffer
        auto stagingBuffer = *factory.createBuffer(GhaBuffer::Descriptor{
            .size        = totalSize,
            .usageFlags  = GhaBuffer::UsageMode::TransferSource,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });

        //VertexBuffer
        vertexBuffer = *factory.createBuffer(GhaBuffer::Descriptor{
            .size        = vertexBufferSize,
            .usageFlags  = GhaBuffer::UsageMode::TransferDestination | GhaBuffer::UsageMode::VertexBuffer,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::VideoMemory,
        });

        //Combined Buffer
        combinedBuffer = *factory.createBuffer(GhaBuffer::Descriptor{
            .size        = totalSize,
            .usageFlags  = GhaBuffer::UsageMode::TransferDestination | GhaBuffer::UsageMode::VertexBuffer | GhaBuffer::UsageMode::IndexBuffer,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::VideoMemory,
        });

        //Map the data into system memory
        stagingBuffer->write(std::data(this->vertices), vertexOffset, vertexBufferSize);
        stagingBuffer->write(std::data(this->indices), indexOffset, indexBufferSize);

        //Transfer the data to video memory
        transferCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->copyBufferToBuffer(*stagingBuffer, 0, *vertexBuffer, 0, vertexBufferSize);
        transferCommandBuffer->copyBufferToBuffer(*stagingBuffer, 0, *combinedBuffer, 0, totalSize);
        transferCommandBuffer->endRecording();

        auto transferQueueFinishedFence{ *factory.createFence({ false }) };

        transferQueue->submit({ TransferSubmitInfo{ .commandBuffers = { transferCommandBuffer } } }, transferQueueFinishedFence.get());

        transferQueueFinishedFence->wait();
        transferQueue->freeCommandBuffer(*transferCommandBuffer);
    }

    Mesh::Mesh(Mesh const &other) = default;

    Mesh::Mesh(Mesh &&other) noexcept = default;

    Mesh &Mesh::operator=(Mesh const &other) = default;

    Mesh &Mesh::operator=(Mesh &&other) noexcept = default;

    Mesh::~Mesh() = default;
}