#pragma once

//#include "Core/ECS/ECSTypes.hpp"
//#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class System{
		//FUNCTIONS
	public:
		virtual ~SystemBase() = default;

		virtual void update(utl::DeltaTime deltaTime) = 0;
	};

}

//#include "System.inl"