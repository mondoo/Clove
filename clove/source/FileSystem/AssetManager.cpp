#include "Clove/FileSystem/AssetManager.hpp"

#include "Clove/ModelLoader.hpp"

namespace garlic::clove {
    AssetPtr<StaticModel> AssetManager::getStaticModel(std::filesystem::path const &filePath) {
        std::string const pathString{ filePath.string() };

        if(!staticModels.contains(pathString)) {
            staticModels[pathString] = AssetPtr<StaticModel>{ filePath, &ModelLoader::loadStaticModel };
        }

        return staticModels.at(pathString);
    }

    AssetPtr<AnimatedModel> AssetManager::getAnimatedModel(std::filesystem::path const &filePath) {
        std::string const pathString{ filePath.string() };

        if(!animatedModels.contains(pathString)) {
            animatedModels[pathString] = AssetPtr<AnimatedModel>{ filePath, &ModelLoader::loadAnimatedModel };
        }

        return animatedModels.at(pathString);
    }
}