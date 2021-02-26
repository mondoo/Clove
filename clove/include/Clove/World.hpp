#pragma once

#include <Clove/ECS/EntityManager.hpp>
#include <filesystem>
#include <vector>

//TEMP: Move the serialisation functions to their classes

namespace garlic::clove {
    class World {
        //VARIABLES
    private:
        EntityManager *manager{ nullptr };

        std::filesystem::path sceneFile;
        std::vector<Entity> knownEntities{};

        //FUNCTIONS
    public:
        //TODO: Ctors
        World() = delete;
        World(EntityManager *manager, std::filesystem::path saveData);

        void save();
        void load();

        //TODO: Inl
        Entity createEntity(std::string_view name) { //TODO: Add name component on Bulb side instead?
            Entity entity{ manager->create() };
            knownEntities.push_back(entity);            
            return entity;
        }

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
            ComponentType &component{ manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...) };
            
            //TODO: Track components?
            
            return component;
        }
    };
}

