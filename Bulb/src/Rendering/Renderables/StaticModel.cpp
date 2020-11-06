#include "Bulb/Rendering/Renderables/StaticModel.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

namespace blb::rnd {
    StaticModel::StaticModel(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Material> material)
        : meshes(std::move(meshes))
        , material(std::move(material)) {
    }

    StaticModel::StaticModel(StaticModel const &other) 
		: material(std::make_shared<Material>(*other.material)){
        meshes.clear();
        for(auto const &mesh : other.meshes) {
            meshes.emplace_back(std::make_shared<Mesh>(*mesh));
        }
    }

    StaticModel::StaticModel(StaticModel &&other) = default;

    StaticModel &StaticModel::operator=(StaticModel const &other) {
        material = std::make_shared<Material>(*other.material);
        meshes.clear();
        for(auto const &mesh : other.meshes) {
            meshes.emplace_back(std::make_shared<Mesh>(*mesh));
        }

        return *this;
    }

    StaticModel &StaticModel::operator=(StaticModel &&other) = default;

    StaticModel::~StaticModel() = default;
}