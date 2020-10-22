#pragma once

#include "Root/Event/EventTypes.hpp"

namespace garlic::inline root {
    class EventContainerBase;
}

namespace garlic::inline root {
    struct EventHandle {
        friend class EventContainerBase;

        //VARIABLES
    private:
        ListenerId id                 = invalidListenerId;
        EventContainerBase* container = nullptr;

        //FUNCTIONS
    public:
        EventHandle();
        EventHandle(ListenerId id, EventContainerBase* container);

        EventHandle(const EventHandle& other) = delete;
        EventHandle(EventHandle&& other) noexcept;

        EventHandle& operator=(const EventHandle& other) = delete;
        EventHandle& operator=(EventHandle&& other) noexcept;

        ~EventHandle();

        void reset();

        ListenerId getId() const;

        bool isValid() const;

        operator ListenerId() const;

    private:
        void clear();
    };
}