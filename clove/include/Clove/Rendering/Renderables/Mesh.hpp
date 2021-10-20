#pragma once

#include "Clove/Rendering/Vertex.hpp"

#include <memory>

namespace clove {
    class GhaBuffer;
    class VertexLayout;
    class VertexBufferData;
    class CommandBuffer;
    class GhaFactory;
}

namespace clove {
    class Mesh {
        //VARIABLES
    private:
        /**
         * @brief Buffer container both the indices and vertices of this mesh.
         */
        std::unique_ptr<GhaBuffer> combinedBuffer;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        size_t vertexOffset{ 0 };
        size_t vertexBufferSize{ 0 };
        size_t indexOffset{ 0 };
        size_t indexBufferSize{ 0 };

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
         * @brief Returns the GhaBuffer containing just the vertices.
         * The lifetime of the buffer is tied to this object.
         * @details This buffer contains the bind pose vertices for the
         * mesh. Before any skinning has taken place.
         */
        inline GhaBuffer *getCombinedBuffer() const;

        /**
         * @brief Returns the offset into the combinedBuffer for the vertices.
         */
        inline size_t getVertexOffset() const;
        inline size_t getVertexBufferSize() const;
        inline size_t getVertexCount() const;

        /**
         * @brief Returns the offset into the combinedBuffer for the indices.
         */
        inline size_t getIndexOffset() const;
        inline size_t getIndexBufferSize() const;
        inline uint32_t getIndexCount() const;
    };
}

#include "Mesh.inl"
