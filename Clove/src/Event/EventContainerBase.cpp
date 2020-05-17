#include "Clove/Event/EventContainerBase.hpp"

#include "Clove/Event/EventHandle.hpp"

namespace clv {
	EventContainerBase::~EventContainerBase() {
		for(auto* handle : handles) {
			handle->clear();
		}
	}
}