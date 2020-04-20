#pragma once

#include "Clove/Utils/DeltaTime.hpp"

namespace blb::ecs{
	class ComponentInterface;
	class World;
}

namespace blb::ecs{
	class System{
		friend class World;

		//FUNCTIONS
	public:
		virtual ~System() = default;

		virtual void preUpdate(World& world){}
		virtual void update(World& world, clv::utl::DeltaTime deltaTime) = 0;
		virtual void postUpdate(World& world){}

		virtual void onComponentCreated(ComponentInterface* component){}
		virtual void onComponentDestroyed(ComponentInterface* component){}
	};
}