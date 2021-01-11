#include "Membrane/Scene.hpp"

namespace garlic::membrane {
    Scene::Scene(std::filesystem::path scenefile) {
        //Load
    }

    Scene::~Scene() {
        //Save???
    }

    clove::Entity Scene::createEntity() {
        return clove::Entity();
    }

    void Scene::save() {
    }
}