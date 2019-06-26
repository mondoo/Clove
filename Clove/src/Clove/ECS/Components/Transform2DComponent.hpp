#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class Transform2DComponent : public Component{
		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x1ae98fd4; //VS Generated GUID

	private:
		math::Vector2f						localPosition = { 0.0f, 0.0f };
		std::pair<math::Vector2f, float>	localRotation = std::pair<math::Vector2f, float>(math::Vector2f(1.0f), 0.0f);
		math::Vector2f						localScale = { 1.0f, 1.0f };

		Transform2DComponent* parent = nullptr;
		std::vector<Transform2DComponent*> children;

		//FUNCTIONS	
	public:
		Transform2DComponent();
		Transform2DComponent(const Transform2DComponent& other) = delete;
		Transform2DComponent(Transform2DComponent&& other) noexcept;
		Transform2DComponent& operator=(const Transform2DComponent& other) = delete;
		Transform2DComponent& operator=(Transform2DComponent&& other) noexcept;
		virtual ~Transform2DComponent();

		void setLocalPosition(const math::Vector2f& inLocalPosition);
		const math::Vector2f& getLocalPosition() const;

		void setLocalRotation(const std::pair<math::Vector2f, float>& inLocalRotation);
		const std::pair<math::Vector2f, float>& getLocalRotation() const;

		void setLocalScale(const math::Vector2f& inLocalScale);
		const math::Vector2f& getLocalScale() const;

		Transform2DComponent* getParent() const;
		void addChild(Transform2DComponent* child);
	};
}
