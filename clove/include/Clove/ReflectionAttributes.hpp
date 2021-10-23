#pragma once

#include <string>
#include <optional>

namespace clove {
    /**
     * @brief Signifies that the class or member wants to be displayed in editor.
     */
    struct EditorVisible {
        std::optional<std::string> name{}; /**< Name of the member/class. If not set then will just use the field's name. */
    };
}