#pragma once

#include "Clove/Rendering/Vertex.hpp"

namespace garlic::clove {
    class GhaBuffer;
    class VertexLayout;
    class VertexBufferData;
    class CommandBuffer;
    class GhaFactory;
}

namespace garlic::clove {
    class Mesh {
        //VARIABLES
    private:
        std::shared_ptr<garlic::clove::GhaBuffer> buffer;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        size_t vertexOffset{ 0 };
        size_t indexOffset{ 0 };

        //FUNCTIONS
    public:
        Mesh() = delete;
        Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices);

        Mesh(Mesh const &other);
        Mesh(Mesh &&other) noexcept;

        Mesh &operator=(Mesh const &other);
        Mesh &operator=(Mesh &&other) noexcept;

        ~Mesh();

        /**
         * @brief Returns a GhaBuffer containing both vertices and indices
         */
        inline std::shared_ptr<garlic::clove::GhaBuffer> const &getGhaBuffer() const;

        /**
         * @brief Returns the offset into the GhaBuffer for the vertices.
         */
        inline size_t getVertexOffset() const;

        /**
         * @brief Returns the offset into the GhaBuffer for the indices.
         */
        inline size_t getIndexOffset() const;
        inline size_t getIndexCount() const;
    };
}

#include "Mesh.inl"
