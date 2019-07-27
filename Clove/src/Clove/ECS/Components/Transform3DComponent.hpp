#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class Transform3DComponent : public Component{
		friend class Transform3DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x3ac0b673; //VS Generated GUID

	private:
		math::Vector3f						position	= { 0.0f, 0.0f, 0.0f };
		std::pair<math::Vector3f, float>	rotation	= std::pair<math::Vector3f, float>(math::Vector3f(1.0f), 0.0f);
		math::Vector3f						scale		= { 1.0f, 1.0f, 1.0f };

		std::optional<math::Vector3f>						desiredPosition;
		std::optional<std::pair<math::Vector3f, float>>	desiredRotation;
		std::optional<math::Vector3f>						desiredScale;

		math::Vector3f						localPosition	= { 0.0f, 0.0f, 0.0f };
		std::pair<math::Vector3f, float>	localRotation	= std::pair<math::Vector3f, float>(math::Vector3f(1.0f), 0.0f);
		math::Vector3f						localScale		= { 1.0f, 1.0f, 1.0f };

		std::optional<math::Vector3f>					desiredLocalPosition;
		std::optional<std::pair<math::Vector3f, float>>	desiredLocalRotation;
		std::optional<math::Vector3f>					desiredLocalScale;

		//TODO: Would like world pos/rot/scale - but this'll do for now
		//math::Matrix4f worldTransformMatrix = math::Matrix4f(1.0f);

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

		const std::pair<math::Vector3f, float>& getRotation() const;
		const std::pair<math::Vector3f, float>& getLocalRotation() const;

		const math::Vector3f& getScale() const;
		const math::Vector3f& getLocalScale() const;

		void setPosition(const math::Vector3f& inPosition);
		void setLocalPosition(const math::Vector3f& inLocalPosition);
		
		void setRotation(const std::pair<math::Vector3f, float>& inRotation);
		void setLocalRotation(const std::pair<math::Vector3f, float>& inLocalRotation);
		
		void setScale(const math::Vector3f& inScale);
		void setLocalScale(const math::Vector3f& inLocalScale);

		//const math::Matrix4f& getWorldTransformMatrix() const;

		//Transform3DComponent* getParent() const;
		void addChild(Transform3DComponent* child);
	};
}
