#pragma once

#include "Clove/Rendering/Material.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"

namespace clove {
    struct StaticModelComponent {
        AssetPtr<StaticModel> model;
        Material material{};
    };
}