#pragma once

#include <Clove/ECS/Entity.hpp>
#include <filesystem>

namespace garlic::membrane {
    /**
     * @brief A scene is like a level or world. It is a self contained object that holds entities.
     */
    class Scene {
        //VARIABLES
    private:
        std::vector<clove::Entity> entities;
        std::filesystem::path sceneFile;

        //FUNCTIONS
    public:
        //TODO: Ctors
        Scene(std::filesystem::path sceneFile);
        ~Scene();

        clove::Entity createEntity();

        void save();
        std::vector<clove::Entity> const &load();
    };
}