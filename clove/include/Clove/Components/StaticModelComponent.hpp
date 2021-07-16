#pragma once

#include "Clove/FileSystem/AssetManager.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"
#include "Clove/SerialisationCommon.hpp"
#include "Clove/Application.hpp"
#include "Clove/Rendering/Material.hpp"

/*
Need some sort of texture type for the assetptr - could it just be a GhaImage?
Material holds asset ptrs to these textures
*/

namespace garlic::clove {
    struct StaticModelComponent {
        AssetPtr<StaticModel> model;
        std::shared_ptr<Material> material{ std::make_shared<Material>() };
    };
}

namespace garlic::clove {
    //TODO: Serialise material (hopefully temp)

    template<>
    inline serialiser::Node serialise(StaticModelComponent const &object) {
        serialiser::Node node{};
        node["path"] = object.model.getPath().string();
        return node;
    }

    template<>
    inline StaticModelComponent deserialise(serialiser::Node const &node) {
        return StaticModelComponent{ Application::get().getAssetManager()->getStaticModel(node["path"].as<std::string>()) };
    }
}