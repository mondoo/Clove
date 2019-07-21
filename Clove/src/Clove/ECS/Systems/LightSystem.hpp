#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform3DComponent.hpp"
#include "Clove/ECS/Components/LightComponent.hpp"

namespace clv::ecs{
	class LightSystem : public System<Transform3DComponent, LightComponent>{
		//FUNCTIONS
	public:
		LightSystem();
		LightSystem(const System& other) = delete;
		LightSystem(LightSystem&& other) noexcept;
		LightSystem& operator=(const LightSystem& other) = delete;
		LightSystem& operator=(LightSystem&& other) noexcept;
		virtual ~LightSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}