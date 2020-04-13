#pragma once

#include "Clove/Core/Utils/DeltaTime.hpp"

namespace tnc::ecs{
	class ComponentInterface;
	class World;
}

namespace tnc::ecs{
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