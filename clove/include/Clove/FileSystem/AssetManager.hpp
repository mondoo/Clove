#pragma once

#include "Clove/FileSystem/AssetPtr.hpp"
#include "Clove/Rendering/Renderables/AnimatedModel.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"

#include <filesystem>
#include <unordered_map>

namespace garlic::clove {
    class VirtualFileSystem;
}

namespace garlic::clove {
    /**
     * @brief Manages the loading of assets.
     */
    class AssetManager {
        //VARIABLES
    private:
        VirtualFileSystem *vfs{ nullptr };

        std::unordered_map<std::string, AssetPtr<StaticModel>> staticModels{};
        std::unordered_map<std::string, AssetPtr<AnimatedModel>> animatedModels{};

        //FUNCTIONS
    public:
        AssetManager() = delete;
        AssetManager(VirtualFileSystem *vfs);

        AssetManager(AssetManager const &other) = delete;
        AssetManager(AssetManager &&other) noexcept;

        AssetManager &operator=(AssetManager const &other) = delete;
        AssetManager &operator=(AssetManager &&other) noexcept;

        ~AssetManager();

        /**
         * @brief Get a StaticModel
         * @param filePath A VirtualFileSystem friendly file path
         * @return An AssetPtr to the StaticModel
         */
        AssetPtr<StaticModel> getStaticModel(std::filesystem::path const &filePath);
        /**
         * @brief Get an AnimatedModel
         * @param filePath A VirtualFileSystem friendly file path
         * @return An AssetPtr to the AnimatedModel
         */
        AssetPtr<AnimatedModel> getAnimatedModel(std::filesystem::path const &filePath);
    };
}