#pragma once

#include "Bulb/Rendering/Material.hpp"
#include "Bulb/Rendering/Vertex.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

namespace clv::gfx {
    class GraphicsBuffer;
    class VertexLayout;
    class VertexBufferData;
    class CommandBuffer;
}

namespace blb::rnd {
    class Mesh {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsBuffer> vertexBuffer;
        std::shared_ptr<clv::gfx::GraphicsBuffer> indexBuffer;
        Material material;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        //FUNCTIONS
    public:
        Mesh() = delete;
        Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory& factory);
        Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, Material material, clv::gfx::GraphicsFactory& factory);

        Mesh(const Mesh& other);
        Mesh(Mesh&& other) noexcept;

        Mesh& operator=(const Mesh& other);
        Mesh& operator=(Mesh&& other) noexcept;

        ~Mesh();

        void setMaterial(Material material);
        const Material& getMaterial() const;

        size_t getIndexCount();

        const std::shared_ptr<clv::gfx::GraphicsBuffer>& getVertexBuffer() const;
        const std::shared_ptr<clv::gfx::GraphicsBuffer>& getIndexBuffer() const;
    };
}
