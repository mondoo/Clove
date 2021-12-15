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
    inline std::string defaultEditorGetValue(void const *const memory, size_t offset, size_t size) {
        //NOTE: Currently assuming everything using this is a float
        return { "test" };
    }

    inline void defaultEditorSetValue(void const *const memory, size_t offset, size_t size, std::string_view value) {
        //NOTE: Currently assuming everything using this is a float
    }
}

namespace clove {
    /**
     * @brief Signifies that this type wants to be displayed in editor as an entity component.
     */
    struct EditorVisibleComponent {
        std::optional<std::string> name{}; /**< Name of the class. If not set then will just use the type's name. */

        std::function<uint8_t *(Entity, EntityManager &)> onEditorCreateComponent{}; /**< Called when the editor wants to create this component. */
        std::function<void(Entity, EntityManager &)> onEditorDestroyComponent{};     /**< Called when the editor wants to destroy this component.*/
    };

    /**
     * @brief Allows a member of a type to be modified in editor.
     */
    struct EditorEditableMember {
        std::optional<std::string> name{}; /**< Name of the member. If not set then will just use the type's name. */

        std::function<std::string(void const *const, size_t, size_t)> onEditorGetValue{ &defaultEditorGetValue };            /**< Called when the editor retrieves the member. Override for custom serialisation logic. */
        std::function<void(void const *const, size_t, size_t, std::string_view)> onEditorSetValue{ &defaultEditorSetValue }; /**< Called when the editor writes to the member. Override for custom serialisation logic. */
    };
}