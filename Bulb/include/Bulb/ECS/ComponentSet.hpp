#pragma once

#include "Bulb/ECS/Component.hpp"

namespace blb::ecs {
    /**
	 * @brief a ComponentSet represents a set of components obtained from an entity.
	 * @details Internally a ComponentSet is a vector of tuples where each tuple 
	 * represents an entity and each element within the tuple is one of the components
	 * of the ComponentTypes variadic argument.
	 * @tparam ComponentTypes The types of the components to be in the set.
	 * @see ComponentPtr.
	 */
    template<typename... ComponentTypes>
    using ComponentSet = std::vector<std::tuple<ComponentPtr<ComponentTypes>...>>;
}