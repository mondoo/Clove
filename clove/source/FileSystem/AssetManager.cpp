#include "Clove/FileSystem/AssetManager.hpp"

#include "Clove/FileSystem/VirtualFileSystem.hpp"
#include "Clove/ModelLoader.hpp"
#include "Clove/TextureLoader.hpp"

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
            staticModels[pathString] = AssetPtr<StaticModel>{ filePath, [fullSystemPath]() {
                                                                 return ModelLoader::loadStaticModel(fullSystemPath);
                                                             } };
        }

        return staticModels.at(pathString);
    }

    AssetPtr<AnimatedModel> AssetManager::getAnimatedModel(std::filesystem::path const &filePath) {
        std::filesystem::path const fullSystemPath{ vfs->resolve(filePath) };
        std::string const pathString{ filePath.string() };

        if(!animatedModels.contains(pathString)) {
            animatedModels[pathString] = AssetPtr<AnimatedModel>{ filePath, [fullSystemPath]() {
                                                                     return ModelLoader::loadAnimatedModel(fullSystemPath);
                                                                 } };
        }

        return animatedModels.at(pathString);
    }

    AssetPtr<Texture> AssetManager::getTexture(std::filesystem::path const &filePath) {
        std::filesystem::path const fullSystemPath{ vfs->resolve(filePath) };
        std::string const pathString{ filePath.string() };

        if(!textures.contains(pathString)) {
            textures[pathString] = AssetPtr<Texture>{ filePath, [fullSystemPath]() {
                                                         return TextureLoader::loadTexture(fullSystemPath).getValue();
                                                     } };
        }

        return textures.at(pathString);
    }
}