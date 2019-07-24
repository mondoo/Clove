#include "clvpch.hpp"
#include "Event.hpp"

namespace clv{
	bool Event::isHandled() const{
		return handled;
	}

	std::string Event::toString() const{
		return getName();
	}

	bool Event::isInCategory(EventCategory category){
		return getCategoryFlags() & category;
	}

	EventDispatcher::EventDispatcher(Event& inEvent)
		: event(inEvent){
	}

	std::ostream& operator<<(std::ostream& os, const Event& e){
		return os << e.toString();
	}

	namespace evt{
		std::vector<std::unique_ptr<InternalEventDispatcherBase>> EventDispatcher::dispatchers;
	}
}