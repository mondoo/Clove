#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/CameraComponent.hpp"

namespace clv::ecs::d3{
	class CameraSystem : public System<TransformComponent, CameraComponent>{
		//FUNCTIONS
	public:
		CameraSystem();
		CameraSystem(const CameraSystem& other) = delete;
		CameraSystem(CameraSystem&& other) noexcept;
		CameraSystem& operator=(const CameraSystem& other) = delete;
		CameraSystem& operator=(CameraSystem&& other) noexcept;
		virtual ~CameraSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}
