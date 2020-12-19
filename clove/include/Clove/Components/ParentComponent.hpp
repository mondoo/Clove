#pragma once

#include <Clove/ECS/Component.hpp>
#include <Clove/ECS/Entity.hpp>

namespace garlic::clove {
    /**
     * @brief Establishes a Parent -> Child relationship to which ever entity this is attached to.
     */
    struct ParentComponent : public Component<ParentComponent> {
        Entity parent{ NullEntity };
    };
}
