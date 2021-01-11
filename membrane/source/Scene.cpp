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

        rootNode["version"] = -1;
        rootNode["sceneid"] = -1;

        for(auto entity : entities) {
            YAML::Node entityData;
            entityData["id"]   = entity;
            entityData["name"] = "Test Entity Name";

            rootNode["Entities"].push_back(entityData);

            YAML::Node component1;
            component1["id"] = 123;
            component1["data"].push_back(1);
            component1["data"].push_back(1);

            entityData["components"].push_back(component1);

            YAML::Node component2;
            component2["id"] = 456;
            component2["data"].push_back("first");
            component2["data"].push_back("second");

            entityData["components"].push_back(component2);

        }

        YAML::Emitter emitter;
        emitter << rootNode;

        //TODO: Using just yaml for now but we'd probably want our own type.
        std::filesystem::path outPath{ std::filesystem::current_path() / "scene.yaml" };

        std::ofstream outSteam{ outPath };
        outSteam << emitter.c_str();
    }
}