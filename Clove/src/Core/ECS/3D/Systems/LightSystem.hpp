#pragma once

#include "Core/ECS/System.hpp"

namespace clv::ecs::_3D{
	class LightSystem : public System{
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