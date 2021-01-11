#include "Membrane/Scene.hpp"

#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Application.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>

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
        //NOTE: this node will be loaded from a file.
        YAML::Node node;
        for(auto entity : entities) {
            node["Entities"].push_back(entity);
        }

        YAML::Emitter emitter;
        emitter << node;

        //TODO: Using just yaml for now but we'd probably want our own type.
        std::filesystem::path outPath{ std::filesystem::current_path() / "scene.yaml" };

        std::ofstream outSteam{ outPath };
        outSteam << emitter.c_str();
    }
}