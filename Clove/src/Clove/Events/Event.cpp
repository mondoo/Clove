#include "clvpch.hpp"
#include "Event.hpp"

namespace clv{
	std::string Event::toString() const{
		return getName();
	}

	EventDispatcher::EventDispatcher(Event& inEvent)
		: event(inEvent){
	}

	std::ostream& operator<<(std::ostream& os, const Event& e){
		return os << e.toString();
	}
}