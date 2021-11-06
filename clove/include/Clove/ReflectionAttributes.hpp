#pragma once

#include <optional>
#include <string>

namespace clove {
    /**
     * @brief Signifies that the class wants to be displayed in editor as an entity component.
     */
    struct EditorVisibleComponent {
        std::optional<std::string> name{}; /**< Name of the class. If not set then will just use the type's name. */
    };

    /**
     * @brief Allows a member of a type to be modified in editor.
     */
    struct EditorEditableMember {
        std::optional<std::string> name{}; /**< Name of the member. If not set then will just use the type's name. */
    };
}