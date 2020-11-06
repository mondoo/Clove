#pragma once

#include "Bulb/Rendering/Vertex.hpp"

namespace clv::gfx {
    class GraphicsBuffer;
    class VertexLayout;
    class VertexBufferData;
    class CommandBuffer;
    class GraphicsFactory;
}

namespace blb::rnd {
    class Mesh {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsBuffer> vertexBuffer;
        std::shared_ptr<clv::gfx::GraphicsBuffer> indexBuffer;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        //FUNCTIONS
    public:
        Mesh() = delete;
        Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory &factory);

        Mesh(Mesh const &other);
        Mesh(Mesh &&other) noexcept;

        Mesh &operator=(Mesh const &other);
        Mesh &operator=(Mesh &&other) noexcept;

        ~Mesh();

        inline size_t getIndexCount();

        inline std::shared_ptr<clv::gfx::GraphicsBuffer> const &getVertexBuffer() const;
        inline std::shared_ptr<clv::gfx::GraphicsBuffer> const &getIndexBuffer() const;
    };
}

#include "Mesh.inl"
