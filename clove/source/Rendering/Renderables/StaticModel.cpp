#include "Clove/Rendering/Renderables/StaticModel.hpp"

#include "Clove/Rendering/Renderables/Mesh.hpp"

namespace clove {
    StaticModel::StaticModel(std::vector<std::shared_ptr<Mesh>> meshes)
        : meshes{ std::move(meshes) } {
    }

    StaticModel::StaticModel(StaticModel const &other) {
        meshes.clear();
        for(auto const &mesh : other.meshes) {
            meshes.emplace_back(std::make_shared<Mesh>(*mesh));
        }
    }

    StaticModel::StaticModel(StaticModel &&other) noexcept = default;

    StaticModel &StaticModel::operator=(StaticModel const &other) {
        meshes.clear();
        for(auto const &mesh : other.meshes) {
            meshes.emplace_back(std::make_shared<Mesh>(*mesh));
        }

        return *this;
    }

    StaticModel &StaticModel::operator=(StaticModel &&other) noexcept = default;

    StaticModel::~StaticModel() = default;
}