#pragma once

#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
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

		virtual void update(utl::DeltaTime deltaTime) = 0;
	};
}