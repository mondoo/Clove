#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform3DComponent.hpp"
#include "Clove/ECS/Components/MeshComponent.hpp"

namespace clv::ecs{
	class Render3DSystem : public System<Transform3DComponent, MeshComponent>{
		//FUNCTIONS
	public:
		Render3DSystem();
		Render3DSystem(const Render3DSystem& other) = delete;
		Render3DSystem(Render3DSystem&& other) noexcept;
		Render3DSystem& operator=(const Render3DSystem& other) = delete;
		Render3DSystem& operator=(Render3DSystem&& other) noexcept;
		virtual ~Render3DSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		math::Matrix4f getTransformWorldMatrix(Transform3DComponent* component);
	};
}
