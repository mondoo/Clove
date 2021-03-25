#pragma once

#include "Clove/ModelLoader.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"
#include "Clove/SerialisationCommon.hpp"

namespace garlic::clove {
    struct StaticModelComponent {
        StaticModel model;
    };
}

namespace garlic::clove {
    template<>
    inline serialiser::Node serialise(StaticModelComponent const &object) {
        serialiser::Node node{};
        node["path"] = object.model.getAssetPath().string();
        return node;
    }

    template<>
    inline StaticModelComponent deserialise(serialiser::Node const &node) {
        return StaticModelComponent{ ModelLoader::loadStaticModel(node["path"].as<std::string>()) };
    }
}