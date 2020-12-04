#pragma once

#include "Clove/Event/EventTypes.hpp"

#include <list>

namespace garlic::clove {
    struct EventHandle;
}

namespace garlic::clove {
    class EventContainerBase {
        friend EventHandle;

        //VARIABLES
    protected:
        inline static ListenerId nextId = 1;

        std::list<EventHandle*> handles;

        //FUNCTI0NS
    public:
        EventContainerBase();

        EventContainerBase(EventContainerBase const& other) = delete;
        EventContainerBase(EventContainerBase&& other) noexcept;

        EventContainerBase& operator=(EventContainerBase const& other) = delete;
        EventContainerBase& operator=(EventContainerBase&& other) noexcept;

        virtual ~EventContainerBase();

        virtual void removeListener(ListenerId id) = 0;
    };
}