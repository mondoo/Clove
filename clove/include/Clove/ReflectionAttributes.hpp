#pragma once

#include <Clove/ECS/Entity.hpp>
#include <Clove/Reflection/Reflection.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace clove {
    class EntityManager;
}

namespace clove {
    /**
     * @brief Signifies that this type wants to be displayed in editor as an entity component.
     */
    struct EditorVisibleComponent {
        std::optional<std::string> name{}; /**< Name of the class. If not set then will just use the type's name. */

        std::function<uint8_t *(Entity, EntityManager &)> onEditorCreateComponent{}; /**< Called when the editor wants to create this component. */
    };

    /**
     * @brief Allows a member of a type to be modified in editor.
     */
    struct EditorEditableMember {
        std::optional<std::string> name{}; /**< Name of the member. If not set then will just use the type's name. */
    };
}