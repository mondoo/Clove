#include "Clove/Event/EventDispatcher.hpp"

namespace garlic::clove {
    EventDispatcher::EventDispatcher() = default;

    EventDispatcher::EventDispatcher(EventDispatcher &&other) noexcept = default;

    EventDispatcher &EventDispatcher::operator=(EventDispatcher &&other) noexcept = default;

    EventDispatcher::~EventDispatcher() = default;
}