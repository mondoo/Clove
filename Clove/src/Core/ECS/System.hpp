#pragma once

#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class ComponentInterface;
	class Manager;
}

namespace clv::ecs{
	class System{
		friend class Manager;

		//VARIABLES
	protected:
		Manager* manager = nullptr;

		//FUNCTIONS
	public:
		virtual ~System() = default;

		virtual void preUpdate(){}
		virtual void update(utl::DeltaTime deltaTime) = 0;
		virtual void postUpdate(){}

		virtual void onComponentCreated(ComponentInterface* component){}
		virtual void onComponentDestroyed(ComponentInterface* component){}
	};
}