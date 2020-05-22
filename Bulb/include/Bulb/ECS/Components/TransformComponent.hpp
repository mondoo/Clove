#pragma once

#include "Bulb/ECS/Component.hpp"

namespace blb::ecs {
	enum class TransformSpace {
		Local,
		World
	};
}

namespace blb::ecs {
	class TransformComponent : public Component<TransformComponent> {
		//VARIABLES
	private:
		clv::mth::vec3f localPosition = { 0.0f, 0.0f, 0.0f };
		clv::mth::quatf localRotation = clv::mth::quatf({ 0.0f, 0.0f, 0.0f });
		clv::mth::vec3f localScale = { 1.0f, 1.0f, 1.0f };

		ComponentPtr<TransformComponent> parent;
		std::vector<ComponentPtr<TransformComponent>> children;

		//FUNCTIONS
	public:
		TransformComponent();

		TransformComponent(const TransformComponent& other);
		TransformComponent(TransformComponent&& other) noexcept;

		TransformComponent& operator=(const TransformComponent& other);
		TransformComponent& operator=(TransformComponent&& other) noexcept;

		~TransformComponent();

		clv::mth::vec3f getPosition(TransformSpace space = TransformSpace::Local) const;
		clv::mth::quatf getRotation(TransformSpace space = TransformSpace::Local) const;
		clv::mth::vec3f getScale(TransformSpace space = TransformSpace::Local) const;

		void setPosition(const clv::mth::vec3f& position, TransformSpace space = TransformSpace::Local);
		void setRotation(const clv::mth::quatf& rotation, TransformSpace space = TransformSpace::Local);
		void setScale(const clv::mth::vec3f& scale, TransformSpace space = TransformSpace::Local);

		clv::mth::vec3f getForward() const;
		clv::mth::vec3f getRight() const;
		clv::mth::vec3f getUp() const;

		clv::mth::mat4f getTransformationMatrix(TransformSpace space);

		ComponentPtr<TransformComponent> getParent() const;

		void addChild(ComponentPtr<TransformComponent> child);
		const std::vector<ComponentPtr<TransformComponent>>& getChildren() const;

	private:
		clv::mth::mat4f getWorldTransformMatrix() const;
		clv::mth::mat4f getLocalTransformMatrix() const;

		clv::mth::vec3f getWorldPosition() const;
		const clv::mth::vec3f& getLocalPosition() const;

		clv::mth::quatf getWorldRotation() const;
		const clv::mth::quatf& getLocalRotation() const;

		clv::mth::vec3f getWorldScale() const;
		const clv::mth::vec3f& getLocalScale() const;

		void setWorldPosition(const clv::mth::vec3f& position);
		void setLocalPosition(const clv::mth::vec3f& position);

		void setWorldRotation(const clv::mth::quatf& rotation);
		void setLocalRotation(const clv::mth::quatf& rotation);

		void setWorldScale(const clv::mth::vec3f& scale);
		void setLocalScale(const clv::mth::vec3f& scale);
	};
}
