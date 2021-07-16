#pragma once

#include "Clove/FileSystem/AssetManager.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"
#include "Clove/SerialisationCommon.hpp"
#include "Clove/Application.hpp"
#include "Clove/Rendering/Material.hpp"

namespace garlic::clove {
    struct StaticModelComponent {
        AssetPtr<StaticModel> model;
        std::shared_ptr<Material> material;
    };
}

namespace garlic::clove {
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