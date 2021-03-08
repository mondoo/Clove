#pragma once

#include <Clove/ECS/EntityManager.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class World {
        //VARIABLES
    private:
        EntityManager *manager{ nullptr };

        std::filesystem::path sceneFile;
        std::vector<Entity> knownEntities{};

        //FUNCTIONS
    public:
        World() = delete;
        World(EntityManager *manager, std::filesystem::path saveData);

        World(World const &other)     = delete;
        World(World &&other) noexcept = delete;

        World &operator=(World const &other) = delete;
        World &operator=(World &&other) noexcept = delete;

        ~World();

        void save();
        void load();

        //TODO: Add name component on Bulb side instead?
        Entity createEntity(std::string_view name);

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&...args);
    };
}

#include "World.inl"