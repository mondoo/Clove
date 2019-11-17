#pragma once

#include "Core/ECS/System.hpp"

namespace clv::ecs::_3D{
	class CameraSystem : public System{
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
