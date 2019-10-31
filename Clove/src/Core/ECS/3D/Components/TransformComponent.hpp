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
		math::Vector3f		position	= { 0.0f, 0.0f, 0.0f };
		math::Quaternionf	rotation	= math::Quaternionf({ 0.0f, 0.0f, 0.0f });
		math::Vector3f		scale		= { 1.0f, 1.0f, 1.0f };

		std::optional<math::Vector3f>		desiredPosition;
		std::optional<math::Quaternionf>	desiredRotation;
		std::optional<math::Vector3f>		desiredScale;

		math::Vector3f		localPosition	= { 0.0f, 0.0f, 0.0f };
		math::Quaternionf	localRotation	= math::Quaternionf({ 0.0f, 0.0f, 0.0f });
		math::Vector3f		localScale		= { 1.0f, 1.0f, 1.0f };

		std::optional<math::Vector3f>		desiredLocalPosition;
		std::optional<math::Quaternionf>	desiredLocalRotation;
		std::optional<math::Vector3f>		desiredLocalScale;

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

		const math::Vector3f& getPosition() const;
		const math::Vector3f& getLocalPosition() const;

		const math::Quaternionf& getRotation() const;
		const math::Quaternionf& getLocalRotation() const;

		const math::Vector3f& getScale() const;
		const math::Vector3f& getLocalScale() const;

		void setPosition(const math::Vector3f& position);
		void setLocalPosition(const math::Vector3f& position);
		
		void setRotation(const math::Quaternionf& rotation);
		void setLocalRotation(const math::Quaternionf& rotation);
		
		void setScale(const math::Vector3f& scale);
		void setLocalScale(const math::Vector3f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		const math::Matrix4f& getWorldTransformMatrix() const;
		const math::Matrix4f& getLocalTransformMatrix() const;
	};
}
