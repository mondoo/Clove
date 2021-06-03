#include "Clove/FileSystem/AssetManager.hpp"

#include "Clove/FileSystem/VirtualFileSystem.hpp"
#include "Clove/ModelLoader.hpp"

namespace garlic::clove {
    AssetManager::AssetManager(VirtualFileSystem *vfs)
        : vfs{ vfs } {
    }

    AssetManager::AssetManager(AssetManager &&other) noexcept = default;

    AssetManager &AssetManager::operator=(AssetManager &&other) noexcept = default;

    AssetManager::~AssetManager() = default;

    AssetPtr<StaticModel> AssetManager::getStaticModel(std::filesystem::path const &filePath) {
        std::filesystem::path const fullSystemPath{ vfs->resolve(filePath) };
        std::string const pathString{ filePath.string() };

        if(!staticModels.contains(pathString)) {
            staticModels[pathString] = AssetPtr<StaticModel>{ fullSystemPath, &ModelLoader::loadStaticModel };
        }

        return staticModels.at(pathString);
    }

    AssetPtr<AnimatedModel> AssetManager::getAnimatedModel(std::filesystem::path const &filePath) {
        std::filesystem::path const fullSystemPath{ vfs->resolve(filePath) };
        std::string const pathString{ filePath.string() };

        if(!animatedModels.contains(pathString)) {
            animatedModels[pathString] = AssetPtr<AnimatedModel>{ fullSystemPath, &ModelLoader::loadAnimatedModel };
        }

        return animatedModels.at(pathString);
    }
}