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