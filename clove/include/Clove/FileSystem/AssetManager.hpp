#pragma once

#include "Clove/FileSystem/AssetPtr.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"
#include "Clove/Rendering/Renderables/AnimatedModel.hpp"

#include <unordered_map>
#include <filesystem>

namespace garlic::clove {
    class AssetManager {
        //VARIABLES
    private:
        std::unordered_map<std::string, AssetPtr<StaticModel>> staticModels{};
        std::unordered_map<std::string, AssetPtr<AnimatedModel>> animatedModels{};

        //FUNCTIONS
    public:
        //TODO: Ctors

        AssetPtr<StaticModel> getStaticModel(std::filesystem::path const &filePath);
        AssetPtr<AnimatedModel> getAnimatedModel(std::filesystem::path const &filePath);
    };
}