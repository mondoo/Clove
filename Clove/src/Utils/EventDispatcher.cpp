#include "Clove/Utils/EventDispatcher.hpp"

namespace clv::utl {
	EventHandle::EventHandle(ListenerId id, std::function<void(ListenerId)> clearFunc, EventContainerBase* container)
		: id(id)
		, clearEvent(std::move(clearFunc)) {
		container->handles.push_back(this);
	}
}