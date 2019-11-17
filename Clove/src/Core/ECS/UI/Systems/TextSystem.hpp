#pragma once

#include "Core/ECS/System.hpp"

namespace clv::ecs::ui{
	class TextSystem : public System{
		//FUNCTIONS
	public:
		//TODO: The rest
		TextSystem();
		~TextSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}