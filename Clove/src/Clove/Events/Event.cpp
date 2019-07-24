#include "clvpch.hpp"
#include "Event.hpp"

namespace clv::evt{
	std::vector<std::unique_ptr<InternalEventDispatcherBase>> EventDispatcher::dispatchers;
}