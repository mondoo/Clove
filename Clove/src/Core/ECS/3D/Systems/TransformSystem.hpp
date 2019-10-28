#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/3D/Components/TransformComponent.hpp"

#include <optional>
#include <tuple>

namespace clv::ecs::_3D{
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
		math::Vector3f getValueFromOptional(std::optional<math::Vector3f>& optional);
		math::Quaternionf getValueFromOptional(std::optional<math::Quaternionf>& optional);

		std::tuple<math::Vector3f, math::Quaternionf, math::Vector3f> breakoutTransforms(math::Matrix4f transformMatrix);
	};
}
