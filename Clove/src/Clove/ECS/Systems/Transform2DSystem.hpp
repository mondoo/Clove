#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform2DComponent.hpp"

namespace clv::ecs{
	class Transform2DSystem : public System<Transform2DComponent>{
		//FUNCTIONS
	public:
		Transform2DSystem();
		Transform2DSystem(const Transform2DSystem& other) = delete;
		Transform2DSystem(Transform2DSystem&& other) noexcept;
		Transform2DSystem& operator=(const Transform2DSystem& other) = delete;
		Transform2DSystem& operator=(Transform2DSystem&& other) noexcept;
		virtual ~Transform2DSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		//math::Matrix4f getTransformWorldMatrix(Transform2DComponent* component);
	};
}
