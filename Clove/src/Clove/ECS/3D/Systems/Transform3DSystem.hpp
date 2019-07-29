#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/3D/Components/Transform3DComponent.hpp"

#include <optional>
#include <tuple>

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
		math::Vector3f getValueFromOptional(std::optional<math::Vector3f>& optional);
		math::Quaternionf getValueFromOptional(std::optional<math::Quaternionf>& optional);

		std::tuple<math::Vector3f, math::Quaternionf, math::Vector3f> breakoutTransforms(math::Matrix4f transformMatrix);
	};
}
