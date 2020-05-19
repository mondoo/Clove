#pragma once

namespace clv {
	using ListenerId = size_t;
	static constexpr ListenerId invalidListenerId = 0;

	template<typename EventType>
	using ListenerFunction = std::function<void(const EventType&)>;
}