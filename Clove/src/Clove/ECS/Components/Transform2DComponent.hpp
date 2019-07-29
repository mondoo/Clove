#pragma once

#include "Clove/ECS/Component.hpp"

#include <optional>

namespace clv::ecs{
	class Transform2DComponent : public Component{
		friend class Transform2DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x1ae98fd4; //VS Generated GUID

	private:
		math::Vector2f	position	= { 0.0f, 0.0f };
		float			rotation	= 0.0f;
		math::Vector2f	scale		= { 1.0f, 1.0f };

		std::optional<math::Vector2f>	desiredPosition;
		std::optional<float>			desiredRotation;
		std::optional<math::Vector2f>	desiredScale;

		math::Vector2f	localPosition	= { 0.0f, 0.0f };
		float			localRotation	= 0.0f;
		math::Vector2f	localScale		= { 1.0f, 1.0f };

		std::optional<math::Vector2f>	desiredLocalPosition;
		std::optional<float>			desiredLocalRotation;
		std::optional<math::Vector2f>	desiredLocalScale;

		math::Matrix3f worldTransformMatrix = math::Matrix4f(1.0f);
		math::Matrix3f localTransformMatrix = math::Matrix4f(1.0f);

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

		const math::Vector2f& getPosition() const;
		const math::Vector2f& getLocalPosition() const;
		
		float getRotation() const;
		float getLocalRotation() const;
		
		const math::Vector2f& getScale() const;
		const math::Vector2f& getLocalScale() const;
		
		void setPosition(const math::Vector2f& position);
		void setLocalPosition(const math::Vector2f& position);

		void setRotation(float rotation);
		void setLocalRotation(float inLocalRotation);

		void setScale(const math::Vector2f& scale);
		void setLocalScale(const math::Vector2f& scale);

		Transform2DComponent* getParent() const;
		void addChild(Transform2DComponent* child);

		const math::Matrix3f& getWorldTransformMatrix() const;
		const math::Matrix3f& getLocalTransformMatrix() const;
	};
}
