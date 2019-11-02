#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Utils/HashString.hpp"

namespace clv::ecs::_3D{
	class TransformComponent : public Component{
		friend class TransformSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(_3D::TransformComponent);

	private:
		math::Vector3f		localPosition	= { 0.0f, 0.0f, 0.0f };
		math::Quaternionf	localRotation	= math::Quaternionf({ 0.0f, 0.0f, 0.0f });
		math::Vector3f		localScale		= { 1.0f, 1.0f, 1.0f };

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

		math::Vector3f getPosition() const;
		const math::Vector3f& getLocalPosition() const;

		math::Quaternionf getRotation() const;
		const math::Quaternionf& getLocalRotation() const;

		math::Vector3f getScale() const;
		const math::Vector3f& getLocalScale() const;

		void setPosition(const math::Vector3f& position);
		void setLocalPosition(const math::Vector3f& position);
		
		void setRotation(const math::Quaternionf& rotation);
		void setLocalRotation(const math::Quaternionf& rotation);
		
		void setScale(const math::Vector3f& scale);
		void setLocalScale(const math::Vector3f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		math::Matrix4f getWorldTransformMatrix() const;
		math::Matrix4f getLocalTransformMatrix() const;
	};
}
