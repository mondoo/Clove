#pragma once

#include "Root/Event/EventManager.hpp"
#include "Root/Event/EventTypes.hpp"

namespace garlic::inline root {
    class EventDispatcher {
        //VARIABLES
    private:
        EventManager manager;

        //FUNCTIONS
    public:
        EventDispatcher();

        EventDispatcher(const EventDispatcher& other) = delete;
        EventDispatcher(EventDispatcher&& other) noexcept;

        EventDispatcher& operator=(const EventDispatcher& other) = delete;
        EventDispatcher& operator=(EventDispatcher&& other) noexcept;

        ~EventDispatcher();

        template<typename EventType>
        [[nodiscard]] EventHandle bindToEvent(ListenerFunction<EventType>&& function);
        template<typename EventType>
        void unbindFromEvent(ListenerId id);

        template<typename EventType>
        void broadCastEvent(const EventType& event);
    };
}

#include "EventDispatcher.inl"