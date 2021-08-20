namespace clove {
    GhaBuffer *Mesh::getVertexBuffer() const {
        return vertexBuffer.get();
    }

    GhaBuffer *Mesh::getCombinedBuffer() const {
        return combinedBuffer.get();
    }

    size_t Mesh::getVertexOffset() const {
        return vertexOffset;
    }

    size_t Mesh::getVertexBufferSize() const {
        return vertexBufferSize;
    }

    size_t Mesh::getVertexCount() const {
        return vertices.size();
    }

    size_t Mesh::getIndexOffset() const {
        return indexOffset;
    }

    size_t Mesh::getIndexCount() const {
        return indices.size();
    }
}