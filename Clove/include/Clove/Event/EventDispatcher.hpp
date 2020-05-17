#pragma once

#include "Clove/Event/EventManager.hpp"
#include "Clove/Event/EventTypes.hpp"

namespace clv {
	class EventDispatcher {
		//VARIABLES
	private:
		EventManager manager;

		//FUNCTIONS
	public:
		EventDispatcher();

		EventDispatcher(const EventDispatcher& other) = delete;
		EventDispatcher(EventDispatcher&& other) noexcept = delete;

		EventDispatcher& operator=(const EventDispatcher& other) = delete;
		EventDispatcher& operator=(EventDispatcher&& other) noexcept = delete;

		~EventDispatcher();

		template<typename EventType>
		[[nodiscard]] EventHandle bindToEvent(ListenerFunction<EventType>&& function);
		template<typename EventType>
		void unbindFromEvent(ListenerId id);

		template<typename EventType>
		void broadCastEvent(EventType& event);
	};
}

#include "EventDispatcher.inl"