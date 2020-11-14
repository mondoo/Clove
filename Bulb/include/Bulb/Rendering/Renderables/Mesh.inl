namespace blb::rnd {
    std::shared_ptr<clv::gfx::GraphicsBuffer> const &Mesh::getGraphicsBuffer() const {
        return buffer;
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