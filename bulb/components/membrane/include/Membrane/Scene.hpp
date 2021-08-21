#pragma once

#include <Clove/ECS/EntityManager.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace membrane {
    class Scene {
        //VARIABLES
    private:
        clove::EntityManager *manager{ nullptr };

        std::vector<clove::Entity> knownEntities{};

        //FUNCTIONS
    public:
        Scene() = delete;
        Scene(clove::EntityManager *manager);

        Scene(Scene const &other)     = delete;
        Scene(Scene &&other) noexcept = delete;

        Scene &operator=(Scene const &other) = delete;
        Scene &operator=(Scene &&other) noexcept = delete;

        ~Scene();

        void save(std::filesystem::path const savePath);
        void load(std::filesystem::path const loadPath);

        inline clove::Entity createEntity();
        inline void deleteEntity(clove::Entity);

        inline std::vector<clove::Entity> getKnownEntities() const;

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(clove::Entity entity, ConstructArgs &&...args);
        template<typename ComponentType>
        ComponentType &getComponent(clove::Entity entity);
        template<typename ComponentType>
        bool hasComponent(clove::Entity entity);
        template<typename ComponentType>
        void removeComponent(clove::Entity entity);

        inline void destroyAllEntities();
    };
}

#include "Scene.inl"