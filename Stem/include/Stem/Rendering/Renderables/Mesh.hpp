#pragma once

#include "Stem/Rendering/Vertex.hpp"

namespace clv::gfx {
    class GraphicsBuffer;
    class VertexLayout;
    class VertexBufferData;
    class CommandBuffer;
    class GraphicsFactory;
}

namespace garlic::inline stem {
    class Mesh {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsBuffer> buffer;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        size_t vertexOffset{ 0 };
        size_t indexOffset{ 0 };

        //FUNCTIONS
    public:
        Mesh() = delete;
        Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, clv::gfx::GraphicsFactory &factory);

        Mesh(Mesh const &other);
        Mesh(Mesh &&other) noexcept;

        Mesh &operator=(Mesh const &other);
        Mesh &operator=(Mesh &&other) noexcept;

        ~Mesh();

        inline std::shared_ptr<clv::gfx::GraphicsBuffer> const &getGraphicsBuffer() const;

        /**
         * @brief Returns the offset into the graphics buffer for the vertices.
         */
        inline size_t getVertexOffset() const;

        /**
         * @brief Returns the offset into the graphics buffer for the indices.
         */
        inline size_t getIndexOffset() const;
        inline size_t getIndexCount() const;
    };
}

#include "Mesh.inl"
