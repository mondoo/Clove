#include "Bulb/Rendering/Renderables/Mesh.hpp"

#include "Clove/Graphics/GraphicsFactory.hpp"

namespace blb::rnd{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory& factory) 
        : Mesh(vertices, indices, Material{ factory }, factory) {
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, Material material, clv::gfx::GraphicsFactory& factory) 
        : vertices(std::move(vertices))
        , indices(std::move(indices))
        , material(std::move(material)){
        const size_t vertexBufferSize = sizeof(blb::rnd::Vertex) * std::size(this->vertices);
        const size_t indexBufferSize  = sizeof(uint16_t) * std::size(this->indices);

        //Staging buffers
        clv::gfx::GraphicsBuffer::Descriptor stagingDescriptor{};
        stagingDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferSource;
        stagingDescriptor.sharingMode = clv::gfx::SharingMode::Exclusive;//Only accessed by the transfer queue
        stagingDescriptor.memoryType  = clv::gfx::MemoryType::SystemMemory;

        stagingDescriptor.size   = vertexBufferSize;
        auto stagingBufferVertex = factory.createBuffer(stagingDescriptor);

        stagingDescriptor.size  = indexBufferSize;
        auto stagingBufferIndex = factory.createBuffer(stagingDescriptor);

        //Vertex buffer
        clv::gfx::GraphicsBuffer::Descriptor vertexDescriptor{};
        vertexDescriptor.size        = vertexBufferSize;
        vertexDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferDestination | clv::gfx::GraphicsBuffer::UsageMode::VertexBuffer;
        vertexDescriptor.sharingMode = clv::gfx::SharingMode::Concurrent; //Accessed by transfer and graphics queue, can be optimised with just Exclusive if transfered between queues
        vertexDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

        vertexBuffer = factory.createBuffer(vertexDescriptor);

        //Index Buffer
        clv::gfx::GraphicsBuffer::Descriptor indexDescriptor{};
        indexDescriptor.size        = indexBufferSize;
        indexDescriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::TransferDestination | clv::gfx::GraphicsBuffer::UsageMode::IndexBuffer;
        indexDescriptor.sharingMode = clv::gfx::SharingMode::Concurrent; //Accessed by transfer and graphics queue, can be optimised with just Exclusive if transfered between queues
        indexDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

        indexBuffer = factory.createBuffer(indexDescriptor);

        //Map the data int system memory
        stagingBufferVertex->map(std::data(this->vertices), vertexBufferSize);
        stagingBufferIndex->map(std::data(this->indices), indexBufferSize);

        //Transfer the data to video memory
        auto transferQueue = factory.createTransferQueue({ clv::gfx::QueueFlags::Transient });

        std::shared_ptr<clv::gfx::TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

        transferCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferVertex, 0, *vertexBuffer, 0, vertexBufferSize);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferIndex, 0, *indexBuffer, 0, indexBufferSize);
        transferCommandBuffer->endRecording();

        transferQueue->submit({ { transferCommandBuffer } });
        transferQueue->freeCommandBuffer(*transferCommandBuffer);
    }

	Mesh::Mesh(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

    void Mesh::setMaterial(Material material) {
        this->material = std::move(material);
    }

    const Material& Mesh::getMaterial() const {
        return material;
    }

	size_t Mesh::getIndexCount() {
        return std::size(indices);
    }

    const std::shared_ptr<clv::gfx::GraphicsBuffer>& Mesh::getVertexBuffer() const {
        return vertexBuffer;
    }

    const std::shared_ptr<clv::gfx::GraphicsBuffer>& Mesh::getIndexBuffer() const {
        return indexBuffer;
    }
}