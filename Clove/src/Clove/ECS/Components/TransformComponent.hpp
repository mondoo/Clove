#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class TransformComponent : public Component{
		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x3ac0b673; //VS Generated GUID

	private:
		math::Vector3f						localPosition = { 0.0f, 0.0f, 0.0f };
		std::pair<math::Vector3f, float>	localRotation = std::pair<math::Vector3f, float>(math::Vector3f(1.0f), 0.0f);
		math::Vector3f						localScale	= { 1.0f, 1.0f, 1.0f };

		//FUNCTIONS
	public:
		TransformComponent();
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept;
		virtual ~TransformComponent();

		void setLocalPosition(const math::Vector3f& inLocalPosition);
		const math::Vector3f& getLocalPosition() const;

		void setLocalRotation(const std::pair<math::Vector3f, float>& inLocalRotation);
		const std::pair<math::Vector3f, float>& getLocalRotation() const;

		void setLocalScale(const math::Vector3f& inLocalScale);
		const math::Vector3f& getLocalScale() const;
	};
}
