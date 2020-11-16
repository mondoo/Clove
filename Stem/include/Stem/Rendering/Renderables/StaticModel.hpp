#pragma once

#include "Stem/Rendering/Material.hpp"

namespace garlic::inline stem {
    class Mesh;
}

namespace garlic::inline stem {
    /**
     * @brief Represents a collection of meshes.
     */
    class StaticModel {
        //VARIABLES
    private:
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::shared_ptr<Material> material;

        //FUNCTIONS
    public:
        StaticModel() = delete;
        StaticModel(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Material> material);

        StaticModel(StaticModel const &other);
        StaticModel(StaticModel &&other);

        StaticModel &operator=(StaticModel const &other);
        StaticModel &operator=(StaticModel &&other);

        ~StaticModel();

        inline void setMaterial(std::shared_ptr<Material> material);
        inline std::shared_ptr<Material> const &getMaterial() const;

        inline std::vector<std::shared_ptr<Mesh>> const &getMeshes() const;

        inline std::shared_ptr<Mesh> &operator[](size_t index);
    };
}

#include "StaticModel.inl"