#include "Root/Event/EventDispatcher.hpp"

namespace garlic::inline root {
    EventDispatcher::EventDispatcher() = default;

    EventDispatcher::EventDispatcher(EventDispatcher&& other) noexcept = default;

    EventDispatcher& EventDispatcher::operator=(EventDispatcher&& other) noexcept = default;

    EventDispatcher::~EventDispatcher() = default;
}