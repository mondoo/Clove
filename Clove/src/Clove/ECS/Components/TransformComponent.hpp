#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class TransformComponent : public Component{
		friend class RenderSystem; //TODO: remove this for the getters and setters

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x3ac0b673; //VS Generated GUID

	private:
		math::Vector3f						position = { 0.0f, 0.0f, 0.0f };
		std::pair<math::Vector3f, float>	rotation = std::pair<math::Vector3f, float>(math::Vector3f(1.0f), 0.0f);
		math::Vector3f						scale	= { 1.0f, 1.0f, 1.0f };

		//FUNCTIONS
	public:
		TransformComponent();
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept;
		virtual ~TransformComponent();

		//TODO: Getters / Setters?
	};
}
