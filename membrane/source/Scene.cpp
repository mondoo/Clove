#include "Membrane/Scene.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <Clove/ModelLoader.hpp>

using namespace garlic::clove;

namespace YAML {
    template<>
    struct convert<vec3f> {
        static Node encode(vec3f const &rhs) {
            Node root{};
            root.SetStyle(EmitterStyle::Flow);
            root.push_back(rhs.x);
            root.push_back(rhs.y);
            root.push_back(rhs.z);
            return root;
        }

        static bool decode(Node const &node, vec3f &rhs) {
            if(!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<quatf> {
        static Node encode(quatf const &rhs) {
            Node root{};
            root.SetStyle(EmitterStyle::Flow);
            root.push_back(rhs.x);
            root.push_back(rhs.y);
            root.push_back(rhs.z);
            root.push_back(rhs.w);
            return root;
        }

        static bool decode(Node const &node, quatf &rhs) {
            if(!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace garlic::membrane {
    //TODO: Probably need a serialiser class to abstract yaml and binary.

    Scene::Scene(std::filesystem::path sceneFile)
        : sceneFile{ std::move(sceneFile) } {
        //TODO: make sure the correct file exention is used.
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

        YAML::Node rootNode;

        rootNode["version"] = -1;
        rootNode["sceneid"] = -1;

        for(auto entity : entities) {
            YAML::Node entityNode;
            entityNode["id"]   = entity;
            entityNode["name"] = "Test";

            if(manager->hasComponent<TransformComponent>(entity)) {
                auto const &comp{ manager->getComponent<TransformComponent>(entity) };

                YAML::Node componentNode;
                componentNode["id"]       = typeid(comp).hash_code();
                componentNode["position"] = comp.position;
                componentNode["rotation"] = comp.rotation;
                componentNode["scale"]    = comp.scale;
                entityNode["components"].push_back(componentNode);
            }
            if(manager->hasComponent<StaticModelComponent>(entity)) {
                auto &comp{ manager->getComponent<StaticModelComponent>(entity) };

                YAML::Node componentNode;
                componentNode["id"] = typeid(comp).hash_code();
                entityNode["components"].push_back(componentNode);
            }
            if(manager->hasComponent<PointLightComponent>(entity)) {
                auto &comp{ manager->getComponent<PointLightComponent>(entity) };

                YAML::Node componentNode;
                componentNode["id"] = typeid(comp).hash_code();
                entityNode["components"].push_back(componentNode);
            }

            rootNode["entities"].push_back(entityNode);
        }

        YAML::Emitter emitter;
        emitter << rootNode;

        std::ofstream outSteam{ sceneFile };
        outSteam << emitter.c_str();
    }

    std::vector<clove::Entity> const &Scene::load() {
        auto *manager{ clove::Application::get().getEntityManager() };
        for(auto entity : entities) {
            manager->destroy(entity);
        }

        YAML::Node scene{ YAML::LoadFile(sceneFile.string()) };

        for(auto &entityNode : scene["entities"]) {
            //TODO: Assign an entity id somehow.
            auto entity{ manager->create() };
            for(auto &componentNode : entityNode["components"]) {
                if(componentNode["id"].as<size_t>() == typeid(TransformComponent).hash_code()) {
                    auto &comp{ manager->addComponent<TransformComponent>(entity) };
                    comp.position = componentNode["position"].as<vec3f>();
                    comp.rotation = componentNode["rotation"].as<quatf>();
                    comp.scale    = componentNode["scale"].as<vec3f>();
                }
                if(componentNode["id"].as<size_t>() == typeid(StaticModelComponent).hash_code()) {
                    auto &comp{ manager->addComponent<StaticModelComponent>(entity, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj")) };
                }
                if(componentNode["id"].as<size_t>() == typeid(PointLightComponent).hash_code()) {
                    auto &comp{ manager->addComponent<PointLightComponent>(entity) };
                }
            }
            entities.push_back(entity);
        }

        return entities;
    }
}