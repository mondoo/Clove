#include "Membrane/Scene.hpp"

#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Application.hpp>

namespace garlic::membrane {
    Scene::Scene(std::filesystem::path scenefile) {
        //Load
    }

    Scene::~Scene() {
        //Save???
    }

    clove::Entity Scene::createEntity() {
        auto entity{ clove::Application::get().getEntityManager()->create() };
        entities.push_back(entity);
        return entity;
    }

    void Scene::save() {
    }
}