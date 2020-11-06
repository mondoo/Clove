namespace blb::rnd {
    size_t Mesh::getIndexCount() {
        return std::size(indices);
    }

    std::shared_ptr<clv::gfx::GraphicsBuffer> const &Mesh::getVertexBuffer() const {
        return vertexBuffer;
    }

    std::shared_ptr<clv::gfx::GraphicsBuffer> const &Mesh::getIndexBuffer() const {
        return indexBuffer;
    }
}