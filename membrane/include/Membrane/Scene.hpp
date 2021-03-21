#pragma once

#include <Clove/ECS/EntityManager.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace garlic::membrane {
    class Scene {
        //VARIABLES
    private:
        clove::EntityManager *manager{ nullptr };

        std::filesystem::path sceneFile;
        std::vector<clove::Entity> knownEntities{};

        //FUNCTIONS
    public:
        Scene() = delete;
        Scene(clove::EntityManager *manager, std::filesystem::path saveData);

        Scene(Scene const &other)     = delete;
        Scene(Scene &&other) noexcept = delete;

        Scene &operator=(Scene const &other) = delete;
        Scene &operator=(Scene &&other) noexcept = delete;

        ~Scene();

        void save();
        void load();

        clove::Entity createEntity();
        inline std::vector<clove::Entity> getKnownEntities() const;

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(clove::Entity entity, ConstructArgs &&...args);
    };
}

#include "Scene.inl"