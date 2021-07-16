#pragma once

#include "Clove/Rendering/Material.hpp"
#include "Clove/Rendering/Renderables/AnimatedModel.hpp"

namespace garlic::clove {
    struct AnimatedModelComponent {
        AnimatedModel model;
        std::shared_ptr<Material> material;
    };
}