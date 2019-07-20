#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform3DComponent.hpp"
#include "Clove/ECS/Components/CameraComponent.hpp"

namespace clv::ecs{
	class CameraSystem : public System<Transform3DComponent, CameraComponent>{
		//FUNCTIONS
	public:
		CameraSystem();
		CameraSystem(const CameraSystem& other) = delete;
		CameraSystem(CameraSystem&& other) noexcept;
		CameraSystem& operator=(const CameraSystem& other) = delete;
		CameraSystem& operator=(CameraSystem&& other) noexcept;
		virtual ~CameraSystem();

		virtual void update(float deltaTime) override;
	};
}
