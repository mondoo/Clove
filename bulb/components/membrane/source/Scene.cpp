#include "Membrane/Scene.hpp"

#include "Membrane/NameComponent.hpp"

#include <Clove/Components/CollisionShapeComponent.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/RigidBodyComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Serialisation/Node.hpp>
#include <Clove/Serialisation/Yaml.hpp>
#include <fstream>
#include <typeinfo>

using namespace clove;

namespace membrane {
    namespace {
        template<typename ComponentType>
        void serialiseComponent(serialiser::Node &entityNode, EntityManager &manager, Entity entity) {
            if(manager.hasComponent<ComponentType>(entity)) {
                auto const &comp{ manager.getComponent<ComponentType>(entity) };

                serialiser::Node componentNode{ serialise(comp) };
                componentNode["id"] = typeid(comp).hash_code();
                entityNode["components"].pushBack(componentNode);
            }
        }
    }

    Scene::Scene(EntityManager *manager)
        : manager{ manager } {
    }

    Scene::~Scene() = default;

    void Scene::save(std::filesystem::path const savePath) {
        serialiser::Node rootNode{};
        for(Entity entity : knownEntities) {
            serialiser::Node entityNode{};
            entityNode["id"] = entity;

            serialiseComponent<NameComponent>(entityNode, *manager, entity);
            serialiseComponent<TransformComponent>(entityNode, *manager, entity);
            serialiseComponent<StaticModelComponent>(entityNode, *manager, entity);
            serialiseComponent<PointLightComponent>(entityNode, *manager, entity);
            serialiseComponent<CollisionShapeComponent>(entityNode, *manager, entity);
            serialiseComponent<RigidBodyComponent>(entityNode, *manager, entity);

            rootNode["entities"].pushBack(entityNode);
        }

        std::ofstream fileStream{ savePath, std::ios::out | std::ios::trunc };
        fileStream << emittYaml(rootNode);
    }

    void Scene::load(std::filesystem::path const loadPath) {
        //Only destroy known entities to avoid deleting meta ones (such as the camera)
        for(Entity entity : knownEntities) {
            manager->destroy(entity);
        }
        knownEntities.clear();

        //File won't exist if we haven't saved anything yet
        if(!std::filesystem::exists(loadPath)) {
            return;
        }

        auto loadResult{ loadYaml(loadPath) };
        serialiser::Node rootNode{ loadResult.getValue() };

        for(auto const &entityNode : rootNode["entities"]) {
            Entity entity{ manager->create() };
            for(auto const &componentNode : entityNode["components"]) {
                if(componentNode["id"].as<size_t>() == typeid(NameComponent).hash_code()) {
                    manager->addComponent<NameComponent>(entity, componentNode.as<NameComponent>());
                } else if(componentNode["id"].as<size_t>() == typeid(TransformComponent).hash_code()) {
                    manager->addComponent<TransformComponent>(entity, componentNode.as<TransformComponent>());
                } else if(componentNode["id"].as<size_t>() == typeid(StaticModelComponent).hash_code()) {
                    manager->addComponent<StaticModelComponent>(entity, componentNode.as<StaticModelComponent>());
                } else if(componentNode["id"].as<size_t>() == typeid(PointLightComponent).hash_code()) {
                    manager->addComponent<PointLightComponent>(entity, componentNode.as<PointLightComponent>());
                } else if(componentNode["id"].as<size_t>() == typeid(CollisionShapeComponent).hash_code()) {
                    manager->addComponent<CollisionShapeComponent>(entity, componentNode.as<CollisionShapeComponent>());
                } else if(componentNode["id"].as<size_t>() == typeid(RigidBodyComponent).hash_code()) {
                    manager->addComponent<RigidBodyComponent>(entity, componentNode.as<RigidBodyComponent>());
                }
            }

            knownEntities.push_back(entity);
        }
    }
}