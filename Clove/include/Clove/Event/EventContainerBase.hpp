#pragma once

#include "Clove/Event/EventTypes.hpp"

namespace clv {
	class EventHandle;
}

namespace clv {
	class EventContainerBase {
		friend class EventHandle;

		//VARIABLES
	protected:
		inline static ListenerId nextId = 1;

		std::vector<EventHandle*> handles;

		//FUNCTI0NS
	public:
		virtual ~EventContainerBase();
	};
}