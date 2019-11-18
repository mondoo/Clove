#pragma once

#include "Core/ECS/Component.hpp"

namespace clv::ecs::_2D{
	class TransformComponent : public Component<_2D::TransformComponent>{
		friend class TransformSystem;

		//VARIABLES
	private:
		math::Vector2f	localPosition	= { 0.0f, 0.0f };
		float			localRotation	= 0.0f;
		math::Vector2f	localScale		= { 1.0f, 1.0f };

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

		math::Vector2f getPosition() const;
		const math::Vector2f& getLocalPosition() const;
		
		float getRotation() const;
		float getLocalRotation() const;
		
		math::Vector2f getScale() const;
		const math::Vector2f& getLocalScale() const;
		
		void setPosition(const math::Vector2f& position);
		void setLocalPosition(const math::Vector2f& position);

		void setRotation(float rotation);
		void setLocalRotation(float inLocalRotation);

		void setScale(const math::Vector2f& scale);
		void setLocalScale(const math::Vector2f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		math::Matrix4f getWorldTransformMatrix() const;
		math::Matrix4f getLocalTransformMatrix() const;
	};
}
