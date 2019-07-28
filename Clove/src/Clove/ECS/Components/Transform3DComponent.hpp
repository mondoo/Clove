#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class Transform3DComponent : public Component{
		friend class Transform3DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x3ac0b673; //VS Generated GUID

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

		math::Matrix4f localTransformMatrix = math::Matrix4f(1.0f);
		math::Matrix4f worldTransformMatrix = math::Matrix4f(1.0f);

		Transform3DComponent* parent = nullptr;
		std::vector<Transform3DComponent*> children;

		//FUNCTIONS
	public:
		Transform3DComponent();
		Transform3DComponent(const Transform3DComponent& other) = delete;
		Transform3DComponent(Transform3DComponent&& other) noexcept;
		Transform3DComponent& operator=(const Transform3DComponent& other) = delete;
		Transform3DComponent& operator=(Transform3DComponent&& other) noexcept;
		virtual ~Transform3DComponent();

		const math::Vector3f& getPosition() const;
		const math::Vector3f& getLocalPosition() const;

		const math::Quaternionf& getRotation() const;
		const math::Quaternionf& getLocalRotation() const;

		const math::Vector3f& getScale() const;
		const math::Vector3f& getLocalScale() const;

		void setPosition(const math::Vector3f& inPosition);
		void setLocalPosition(const math::Vector3f& inLocalPosition);
		
		void setRotation(const math::Quaternionf& inRotation);
		void setLocalRotation(const math::Quaternionf& inLocalRotation);
		
		void setScale(const math::Vector3f& inScale);
		void setLocalScale(const math::Vector3f& inLocalScale);

		Transform3DComponent* getParent() const;
		void addChild(Transform3DComponent* child);
	};
}
