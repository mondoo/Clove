#pragma once

#include "Clove/Event/EventTypes.hpp"

namespace clv {
	class EventContainerBase;
}

namespace clv {
	struct EventHandle {
		friend class EventContainerBase;

		//VARIABLES
	private:
		ListenerId id = invalidListenerId;
		std::function<void(ListenerId)> clearEvent;

		//FUNCTIONS
	public:
		EventHandle() = delete;
		EventHandle(ListenerId id, std::function<void(ListenerId)> clearFunc, EventContainerBase* container);

		EventHandle(const EventHandle& other) = delete;
		EventHandle(EventHandle&& other) noexcept;

		EventHandle& operator=(const EventHandle& other) = delete;
		EventHandle& operator=(EventHandle&& other) noexcept;

		~EventHandle();

		void reset();

		bool isValid() const;

		operator ListenerId() const;

	private:
		void clear();
	};
}