#pragma once

#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class System{
		//FUNCTIONS
	public:
		virtual ~System() = default;

		virtual void update(utl::DeltaTime deltaTime) = 0;
	};
}