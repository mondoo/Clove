#pragma once

#include "Root/Event/EventTypes.hpp"

#include <list>

namespace garlic::inline root {
    class EventHandle;
}

namespace garlic::inline root {
    class EventContainerBase {
        friend class EventHandle;

        //VARIABLES
    protected:
        inline static ListenerId nextId = 1;

        std::list<EventHandle*> handles;

        //FUNCTI0NS
    public:
        EventContainerBase();

        EventContainerBase(const EventContainerBase& other) = delete;
        EventContainerBase(EventContainerBase&& other) noexcept;

        EventContainerBase& operator=(const EventContainerBase& other) = delete;
        EventContainerBase& operator=(EventContainerBase&& other) noexcept;

        virtual ~EventContainerBase();

        virtual void removeListener(ListenerId id) = 0;
    };
}