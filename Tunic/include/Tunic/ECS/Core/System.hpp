#pragma once

#include "Clove/Core/Utils/DeltaTime.hpp"

namespace tnc::ecs{
	class ComponentInterface;
	class Manager;
}

namespace tnc::ecs{
	class System{
		friend class Manager;

		//VARIABLES
	protected:
		Manager* manager = nullptr;

		//FUNCTIONS
	public:
		virtual ~System() = default;

		virtual void preUpdate(){}
		virtual void update(clv::utl::DeltaTime deltaTime) = 0;
		virtual void postUpdate(){}

		virtual void onComponentCreated(ComponentInterface* component){}
		virtual void onComponentDestroyed(ComponentInterface* component){}
	};
}