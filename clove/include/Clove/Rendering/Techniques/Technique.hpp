#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <set>

namespace garlic::clove {
    class Mesh;
    class ForwardRenderer3D;
}

namespace garlic::clove {
    /**
     * @brief Describes the steps involved to render geometry in a certain way.
     * @details A Technique's main responsibility is distribute a mesh into the 
     * the correct individual GeometryPasses. For example a lighting technique will
     * add a mesh into a shadow pass and then a colour pass.
     */
    struct Technique{
        std::set<GeometryPass::Id> passIds{};
    };
}