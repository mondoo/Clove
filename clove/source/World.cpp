#include "Clove/World.hpp"

#include <Clove/Serialisation/Yaml.hpp>
#include <fstream>

namespace garlic::clove {
    World::World(EntityManager *manager, std::filesystem::path saveData)
        : manager{ manager }
        , sceneFile{ std::move(saveData) } {
        auto loadResult{ loadYaml(sceneFile) };
        if(!loadResult.hasValue()) {
            if(loadResult.getError() != LoadError::BadFile) {
                //TODO: Error handling
            }
        } else {
            rootNode = std::move(loadResult.getValue());
        }
    }

    void World::save() {
        std::ofstream fileStream{ sceneFile, std::ios::out | std::ios::trunc };

        fileStream << emittYaml(rootNode);
    }

    void World::load() {
        //TODO:
    }
}