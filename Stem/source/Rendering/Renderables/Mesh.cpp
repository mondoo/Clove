#include "Stem/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>

namespace garlic::inline stem {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory &factory)
        : vertices(std::move(vertices))
        , indices(std::move(indices)) {
        using namespace clv::gfx;

        size_t const vertexBufferSize{ sizeof(Vertex) * std::size(this->vertices) };
        size_t const indexBufferSize{ sizeof(uint16_t) * std::size(this->indices) };
        size_t const totalSize{ vertexBufferSize + indexBufferSize };

        vertexOffset = 0;
        indexOffset  = vertexBufferSize;

        auto transferQueue = factory.createTransferQueue({ QueueFlags::Transient });

        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

        //Staging buffer
        auto stagingBuffer = factory.createBuffer(GraphicsBuffer::Descriptor{
            .size        = totalSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferSource,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });

        //Buffer
        buffer = factory.createBuffer(GraphicsBuffer::Descriptor{
            .size        = totalSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferDestination | GraphicsBuffer::UsageMode::VertexBuffer | GraphicsBuffer::UsageMode::IndexBuffer,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::VideoMemory,
        });

        //Map the data into system memory
        stagingBuffer->write(std::data(this->vertices), vertexOffset, vertexBufferSize);
        stagingBuffer->write(std::data(this->indices), indexOffset, indexBufferSize);

        //Transfer the data to video memory
        transferCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->copyBufferToBuffer(*stagingBuffer, 0, *buffer, 0, totalSize);
        transferCommandBuffer->endRecording();

        transferQueue->submit({ { transferCommandBuffer } });
        transferQueue->freeCommandBuffer(*transferCommandBuffer);
    }

    Mesh::Mesh(Mesh const &other) = default;

    Mesh::Mesh(Mesh &&other) noexcept = default;

    Mesh &Mesh::operator=(Mesh const &other) = default;

    Mesh &Mesh::operator=(Mesh &&other) noexcept = default;

    Mesh::~Mesh() = default;
}