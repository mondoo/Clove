#pragma once

#include "Clove/Utils/DeltaTime.hpp"

namespace clv {
	class EventDispatcher;
}

namespace blb::ecs {
	class ComponentInterface;
	class World;
}

namespace blb::ecs {
	/**
	 * The base class for all Systems
	 */
	class System {
		friend class World;

		//FUNCTIONS
	public:
		virtual ~System() = default;

		/**
		 * Gets called after the System is initialised. Override to bind to any events in the scope of the World
		 * @param dispatcher The EventDispatcher inside the World
		 * @see EventDispatcher
		 */
		virtual void registerToEvents(clv::EventDispatcher& dispatcher) {}

		/**
		 * Called before update
		 * @param world The World that owns this System
		 */
		virtual void preUpdate(World& world) {}
		/**
		 * Called once per frame
		 * @param world The World that owns this System
		 * @param deltaTime The time passed since the last frame
		 */
		virtual void update(World& world, clv::utl::DeltaTime deltaTime) = 0;
		/**
		 * Called after update
		 * @param world The World that owns this System
		 */
		virtual void postUpdate(World& world) {}
	};
}
