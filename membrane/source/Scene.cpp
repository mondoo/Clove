#include "Membrane/Scene.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace garlic::clove;

namespace YAML {
    template<>
    struct convert<TransformComponent> {
        static Node encode(TransformComponent const &rhs) {
            Node root;

            //Required?

            return root;
        }

        static bool decode(Node const &node, TransformComponent &rhs) {
            //TODO:
            return false;
        }
    };
}

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
        auto *manager{ clove::Application::get().getEntityManager() };

        YAML::Emitter emitter;

        emitter << YAML::BeginMap;
        emitter << YAML::Key << "version" << YAML::Value << -1;
        emitter << YAML::Key << "sceneid" << YAML::Value << -1;
        emitter << YAML::Key << "entities";
        emitter << YAML::BeginSeq;
        for(auto entity : entities) {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "id" << YAML::Value << entity;
            emitter << YAML::Key << "name" << YAML::Value << "Test";
            emitter << YAML::Key << "components";
            emitter << YAML::BeginSeq;
            if(manager->hasComponent<TransformComponent>(entity)) {
                auto &comp{ manager->getComponent<TransformComponent>(entity) };
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "id" << YAML::Value << typeid(comp).hash_code();
                emitter << YAML::Key << "data";
                emitter << YAML::BeginSeq;
                emitter << YAML::Flow << std::vector{ comp.position.x, comp.position.y, comp.position.z };
                emitter << YAML::Flow << std::vector{ comp.rotation.x, comp.rotation.y, comp.rotation.z, comp.rotation.w };
                emitter << YAML::Flow << std::vector{ comp.scale.x, comp.scale.y, comp.scale.z };
                emitter << YAML::EndSeq;
                emitter << YAML::EndMap;
            }
            if(manager->hasComponent<StaticModelComponent>(entity)) {
                auto &comp{ manager->getComponent<StaticModelComponent>(entity) };
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "id" << YAML::Value << typeid(comp).hash_code();
                emitter << YAML::EndMap;
            }
            if(manager->hasComponent<PointLightComponent>(entity)) {
                auto &comp{ manager->getComponent<PointLightComponent>(entity) };
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "id" << YAML::Value << typeid(comp).hash_code();
                emitter << YAML::EndMap;
            }
            emitter << YAML::EndSeq;
            emitter << YAML::EndMap;
        }
        emitter << YAML::EndSeq;
        emitter << YAML::EndMap;

        //TEMP: Testing using nodes:
        YAML::Node test{ YAML::NodeType::Map };
        test["data"] = std::vector{ 1, 3, 4 };
        emitter << test;

        //TODO: Using just yaml for now but we'd probably want our own type.
        std::filesystem::path outPath{ std::filesystem::current_path() / "scene.yaml" };

        std::ofstream outSteam{ outPath };
        outSteam << emitter.c_str();
    }
}