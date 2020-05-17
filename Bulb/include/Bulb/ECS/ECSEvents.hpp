#pragma once

namespace blb::ecs {
	template<typename ComponentType>
	struct ComponentAddedEvent {
		ComponentType* component = nullptr;
	};

	template<typename ComponentType>
	struct ComponentRemovedEvent {
		ComponentType* component = nullptr;
	};
}