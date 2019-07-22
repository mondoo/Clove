#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform3DComponent.hpp"

namespace clv::ecs{
	class Transform3DSystem : public System<Transform3DComponent>{
		//FUNCTIONS
	public:
		Transform3DSystem();
		Transform3DSystem(const Transform3DSystem& other) = delete;
		Transform3DSystem(Transform3DSystem&& other) noexcept;
		Transform3DSystem& operator=(const Transform3DSystem& other) = delete;
		Transform3DSystem& operator=(Transform3DSystem&& other) noexcept;
		virtual ~Transform3DSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		math::Matrix4f getTransformWorldMatrix(Transform3DComponent* component);
	};
}
