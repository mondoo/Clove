#pragma once

#include "Clove/FileSystem/AssetPtr.hpp"
#include "Clove/Rendering/Renderables/AnimatedModel.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"

#include <filesystem>
#include <unordered_map>

namespace garlic::clove {
    /**
     * @brief Manages the loading of assets from disk.
     */
    class AssetManager {
        //VARIABLES
    private:
        std::unordered_map<std::string, AssetPtr<StaticModel>> staticModels{};
        std::unordered_map<std::string, AssetPtr<AnimatedModel>> animatedModels{};

        //FUNCTIONS
    public:
        AssetManager();

        AssetManager(AssetManager const &other) = delete;
        AssetManager(AssetManager &&other) noexcept;

        AssetManager &operator=(AssetManager const &other) = delete;
        AssetManager &operator=(AssetManager &&other) noexcept;

        ~AssetManager();

        AssetPtr<StaticModel> getStaticModel(std::filesystem::path const &filePath);
        AssetPtr<AnimatedModel> getAnimatedModel(std::filesystem::path const &filePath);
    };
}