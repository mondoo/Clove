#include "Root/Event/EventContainerBase.hpp"

#include "Root/Event/EventHandle.hpp"

namespace garlic::inline root {
    EventContainerBase::EventContainerBase() = default;

    EventContainerBase::EventContainerBase(EventContainerBase&& other) noexcept {
        handles = std::move(other.handles);

        for(auto* handle : handles) {
            handle->container = this;
        }
    }

    EventContainerBase& EventContainerBase::operator=(EventContainerBase&& other) noexcept {
        handles = std::move(other.handles);

        for(auto* handle : handles) {
            handle->container = this;
        }

        return *this;
    }

    EventContainerBase::~EventContainerBase() {
        for(auto* handle : handles) {
            handle->clear();
        }
    }
}