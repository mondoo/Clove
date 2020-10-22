#pragma once

#include "Root/Event/EventContainerBase.hpp"
#include "Root/Event/EventHandle.hpp"
#include "Root/Event/EventTypes.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace garlic::inline root {
    class EventManager {
        //TYPES
    private:
        template<typename EventType>
        class EventContainer : public EventContainerBase {
            //TYPES
        private:
            using ContainerFunctionType = ListenerFunction<EventType>;

            //VARIABLES
        private:
            std::unordered_map<ListenerId, size_t> listenerIdToIndexMap;
            std::vector<ContainerFunctionType> listeners;

            //FUNCTIONS
        public:
            EventContainer();

            EventContainer(const EventContainer& other) = delete;
            EventContainer(EventContainer&& other) noexcept;

            EventContainer& operator=(const EventContainer& other) = delete;
            EventContainer& operator=(EventContainer&& other) noexcept;

            ~EventContainer();

            [[nodiscard]] EventHandle addListener(ContainerFunctionType function);
            void removeListener(ListenerId id) final;

            typename std::vector<ContainerFunctionType>::iterator begin();
            typename std::vector<ContainerFunctionType>::const_iterator begin() const;

            typename std::vector<ContainerFunctionType>::iterator end();
            typename std::vector<ContainerFunctionType>::const_iterator end() const;
        };

        //VARIABLES
    private:
        std::unordered_map<size_t, std::unique_ptr<EventContainerBase>> containers;

        //FUNCTIONS
    public:
        EventManager();

        EventManager(const EventManager& other) = delete;
        EventManager(EventManager&& other) noexcept;

        EventManager& operator=(const EventManager& other) = delete;
        EventManager& operator=(EventManager&& other) noexcept;

        ~EventManager();

        template<typename EventType>
        EventContainer<EventType>& getEventContainer();
    };
}

#include "EventManager.inl"