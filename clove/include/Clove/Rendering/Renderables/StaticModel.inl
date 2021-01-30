namespace garlic::clove {
    void StaticModel::setMaterial(std::shared_ptr<Material> material) {
        this->material = std::move(material);
    }

    std::shared_ptr<Material> const &StaticModel::getMaterial() const {
        return material;
    }

    std::vector<std::shared_ptr<Mesh>> const &StaticModel::getMeshes() const {
        return meshes;
    }

    std::vector<Technique> const &StaticModel::getTechniques() const {
        return techniques;
    }

    std::shared_ptr<Mesh> &StaticModel::operator[](size_t index) {
        return meshes[index];
    }
}