#pragma once

#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Serialisation/Node.hpp>
#include <filesystem>

//TEMP: Move the serialisation functions to their classes

namespace garlic::clove {
    class World {
        //VARIABLES
    private:
        EntityManager *manager{ nullptr };

        std::filesystem::path sceneFile;
        serialiser::Node rootNode;

        //FUNCTIONS
    public:
        //TODO: Ctors
        World() = delete;
        World(EntityManager *manager, std::filesystem::path saveData);

        void save();
        void load();

        //TODO: Inl
        Entity createEntity(std::string_view name) {
            Entity entity{ manager->create() };

            serialiser::Node entityNode{};
            entityNode["id"] = entity;
            entityNode["name"] = name;
            rootNode["entities"].pushBack(entityNode);
            
            return entity;
        }

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
            ComponentType &component{ manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...) };
            for(serialiser::Node &entityNode : rootNode["entities"]){
                if(entityNode["id"].as<Entity>() == entity){
                    serialiser::Node componentNode{};
                    componentNode["id"] = std::to_string(typeid(ComponentType).hash_code());
                    componentNode["data"] = component;
                    entityNode["components"].pushBack(componentNode);
                }
            }
            
            return component;
        }
    };
}

