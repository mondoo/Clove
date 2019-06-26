#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform2DComponent.hpp"
#include "Clove/ECS/Components/Renderable2DComponent.hpp"

namespace clv::ecs{
	class Render2DSystem : public System<Transform2DComponent, Renderable2DComponent>{
		//VARIABLES
	public:
		static constexpr SystemID ID = 0xfa8540b2; //VS Generated GUID

	private:
		//...
	};
}
