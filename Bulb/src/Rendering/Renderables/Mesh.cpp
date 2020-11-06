#include "Bulb/Rendering/Renderables/Mesh.hpp"

#include "Clove/Graphics/GraphicsFactory.hpp"

namespace blb::rnd {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory &factory)
        : vertices(std::move(vertices))
        , indices(std::move(indices)){
        using namespace clv::gfx;

        size_t const vertexBufferSize = sizeof(Vertex) * std::size(this->vertices);
        size_t const indexBufferSize  = sizeof(uint16_t) * std::size(this->indices);

        auto transferQueue = factory.createTransferQueue({ QueueFlags::Transient });

        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

        //Staging buffers
        GraphicsBuffer::Descriptor stagingDescriptor{
            .usageFlags  = GraphicsBuffer::UsageMode::TransferSource,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        };

        stagingDescriptor.size   = vertexBufferSize;
        auto stagingBufferVertex = factory.createBuffer(stagingDescriptor);

        stagingDescriptor.size  = indexBufferSize;
        auto stagingBufferIndex = factory.createBuffer(std::move(stagingDescriptor));

        //Vertex buffer
        vertexBuffer = factory.createBuffer(GraphicsBuffer::Descriptor{
            .size        = vertexBufferSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferDestination | GraphicsBuffer::UsageMode::VertexBuffer,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::VideoMemory,
        });

        //Index Buffer
        indexBuffer = factory.createBuffer(GraphicsBuffer::Descriptor{
            .size        = indexBufferSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferDestination | GraphicsBuffer::UsageMode::IndexBuffer,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::VideoMemory,
        });

        //Map the data into system memory
        stagingBufferVertex->write(std::data(this->vertices), 0, vertexBufferSize);
        stagingBufferIndex->write(std::data(this->indices), 0, indexBufferSize);

        //Transfer the data to video memory
        transferCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferVertex, 0, *vertexBuffer, 0, vertexBufferSize);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferIndex, 0, *indexBuffer, 0, indexBufferSize);
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