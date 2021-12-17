namespace clove {
    std::vector<std::shared_ptr<Mesh>> const &StaticModel::getMeshes() const {
        return meshes;
    }

    std::shared_ptr<Mesh> &StaticModel::operator[](size_t index) {
        return meshes[index];
    }
}