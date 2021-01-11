#include "Membrane/Scene.hpp"

#include <Clove/Application.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace garlic::membrane {
    Scene::Scene(std::filesystem::path scenefile) {
        //Load
    }

    Scene::~Scene() {
        //Save??? Maybe like scene_backup instead of overwriting main file.
    }

    clove::Entity Scene::createEntity() {
        auto entity{ clove::Application::get().getEntityManager()->create() };
        entities.push_back(entity);
        return entity;
    }

    //TODO: Nodes should be built when created. This should just emitt to file.
    void Scene::save() {
        //NOTE: this node will be loaded from a file.
        YAML::Node rootNode;

        rootNode["Version"] = 0;
        rootNode["SceneId"] = 1234567890;

        for(auto entity : entities) {
            YAML::Node entityData;
            entityData["id"]   = entity;
            entityData["name"] = "Test Entity Name";

            rootNode["Entities"].push_back(entityData);

            YAML::Node components;
            components["TestComponent_1"] = std::vector<float>{ 100.3f, 29.1f, 999.0f };
            components["TestComponent_2"] = "Name: XYZ";

            entityData["components"].push_back(components);
        }

        YAML::Emitter emitter;
        emitter << rootNode;

        //TODO: Using just yaml for now but we'd probably want our own type.
        std::filesystem::path outPath{ std::filesystem::current_path() / "scene.yaml" };

        std::ofstream outSteam{ outPath };
        outSteam << emitter.c_str();
    }
}