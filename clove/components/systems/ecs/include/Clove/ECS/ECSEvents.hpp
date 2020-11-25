#pragma once

namespace garlic::clove {
    template<typename ComponentType>
    struct ComponentAddedEvent {
        ComponentType *component{ nullptr };
    };

    template<typename ComponentType>
    struct ComponentRemovedEvent {
        ComponentType *component{ nullptr };
    };
}