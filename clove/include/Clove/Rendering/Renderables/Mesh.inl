namespace garlic::clove {
    std::shared_ptr<GhaBuffer> const &Mesh::getVertexBuffer() const {
        return vertexBuffer;
    }

    std::shared_ptr<GhaBuffer> const &Mesh::getCombinedBuffer() const {
        return combinedBuffer;
    }

    size_t Mesh::getVertexOffset() const {
        return vertexOffset;
    }

    size_t Mesh::getIndexOffset() const {
        return indexOffset;
    }

    size_t Mesh::getIndexCount() const {
        return std::size(indices);
    }
}