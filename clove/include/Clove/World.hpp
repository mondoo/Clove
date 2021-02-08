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
            //TODO: Handle string conversion?
            rootNode["entities"]["id"] = entity;
            rootNode["entities"]["name"] = name; 
            return entity;
        }

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
            ComponentType &component{ manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...) };
            rootNode["entities"][std::to_string(entity)]["components"][std::to_string(typeid(ComponentType).hash_code())] = component;
            return component;
        }
    };
}

