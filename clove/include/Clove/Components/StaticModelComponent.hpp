#pragma once

#include "Clove/FileSystem/AssetManager.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"
#include "Clove/SerialisationCommon.hpp"
#include "Clove/Application.hpp"

namespace garlic::clove {
    struct StaticModelComponent {
        AssetPtr<StaticModel> model;
    };
}

namespace garlic::clove {
    template<>
    inline serialiser::Node serialise(StaticModelComponent const &object) {
        serialiser::Node node{};
        node["path"] = object.model.getPath().string(); //TODO: This returns the full path and not the VFS path
        return node;
    }

    template<>
    inline StaticModelComponent deserialise(serialiser::Node const &node) {
        return StaticModelComponent{ Application::get().getAssetManager()->getStaticModel(node["path"].as<std::string>()) };
    }
}