#include "Clove/Event/EventHandle.hpp"

#include "Clove/Event/EventContainerBase.hpp"

namespace clv{
	EventHandle::EventHandle(ListenerId id, std::function<void(ListenerId)> clearFunc, EventContainerBase* container)
		: id(id)
		, clearEvent(std::move(clearFunc)) {
		container->handles.push_back(this);
	}

	EventHandle::EventHandle(EventHandle&& other) noexcept = default;
	
	EventHandle& EventHandle::operator=(EventHandle&& other) noexcept = default;
	
	EventHandle::~EventHandle() {
		reset();
	}

	void EventHandle::reset() {
		if(isValid()) {
			clearEvent(id);

			clear();
		}
	}

	bool EventHandle::isValid() const {
		return clearEvent != nullptr && id != invalidListenerId;
	}

	EventHandle::operator ListenerId() const {
		return id;
	}

	void EventHandle::clear() {
		clearEvent = nullptr;
		id = invalidListenerId;
	}
}