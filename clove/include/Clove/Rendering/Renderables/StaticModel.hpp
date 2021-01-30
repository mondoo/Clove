#pragma once

#include "Clove/Rendering/Material.hpp"

//TEMP?
#include "Clove/Rendering/Techniques/Technique.hpp"

#include <memory>
#include <vector>

namespace garlic::clove {
    class Mesh;
    //class TechniqueInterface;
}

namespace garlic::clove {
    /**
     * @brief Represents a collection of meshes.
     */
    class StaticModel {
        //VARIABLES
    public:
        //TEMP: public
        std::vector<Technique> techniques{{}};

    private:
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::shared_ptr<Material> material;

        //std::vector<std::unique_ptr<TechniqueInterface>> techniques;

        //FUNCTIONS
    public:
        StaticModel() = delete;
        StaticModel(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Material> material);

        StaticModel(StaticModel const &other);
        StaticModel(StaticModel &&other) noexcept;

        StaticModel &operator=(StaticModel const &other);
        StaticModel &operator=(StaticModel &&other) noexcept;

        ~StaticModel();

        inline void setMaterial(std::shared_ptr<Material> material);
        inline std::shared_ptr<Material> const &getMaterial() const;

        inline std::vector<std::shared_ptr<Mesh>> const &getMeshes() const;

        inline std::shared_ptr<Mesh> &operator[](size_t index);
    };
}

#include "StaticModel.inl"