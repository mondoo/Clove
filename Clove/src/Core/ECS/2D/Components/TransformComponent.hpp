#pragma once

#include "Core/ECS/Component.hpp"

namespace clv::ecs::d2{
	class TransformComponent : public Component{
		friend class TransformSystem;

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

		math::Matrix4f worldTransformMatrix = math::Matrix4f(1.0f);
		math::Matrix4f localTransformMatrix = math::Matrix4f(1.0f);

		TransformComponent* parent = nullptr;
		std::vector<TransformComponent*> children;

		//FUNCTIONS	
	public:
		TransformComponent();
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept;
		virtual ~TransformComponent();

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

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		const math::Matrix4f& getWorldTransformMatrix() const;
		const math::Matrix4f& getLocalTransformMatrix() const;
	};
}
