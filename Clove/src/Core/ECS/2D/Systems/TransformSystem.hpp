#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/2D/Components/TransformComponent.hpp"

namespace clv::ecs::_2D{
	class TransformSystem : public System<TransformComponent>{
		//FUNCTIONS
	public:
		TransformSystem();
		TransformSystem(const TransformSystem& other) = delete;
		TransformSystem(TransformSystem&& other) noexcept;
		TransformSystem& operator=(const TransformSystem& other) = delete;
		TransformSystem& operator=(TransformSystem&& other) noexcept;
		virtual ~TransformSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		math::Vector2f getValueFromOptional(std::optional<math::Vector2f>& optional);
		float getValueFromOptional(std::optional<float>& optional);

		std::tuple<math::Vector2f, float, math::Vector2f> breakoutTransforms(math::Matrix4f transformMatrix);
	};
}
