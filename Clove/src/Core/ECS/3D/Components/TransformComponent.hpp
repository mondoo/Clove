#pragma once

#include "Core/ECS/Component.hpp"

namespace clv::ecs::_3D{
	class TransformComponent : public Component<_3D::TransformComponent>{
		//VARIABLES
	private:
		mth::vec3f	localPosition	= { 0.0f, 0.0f, 0.0f };
		mth::quatf	localRotation	= mth::quatf({ 0.0f, 0.0f, 0.0f });
		mth::vec3f	localScale		= { 1.0f, 1.0f, 1.0f };

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

		mth::vec3f getPosition() const;
		const mth::vec3f& getLocalPosition() const;

		mth::quatf getRotation() const;
		const mth::quatf& getLocalRotation() const;

		mth::vec3f getScale() const;
		const mth::vec3f& getLocalScale() const;

		void setPosition(const mth::vec3f& position);
		void setLocalPosition(const mth::vec3f& position);
		
		void setRotation(const mth::quatf& rotation);
		void setLocalRotation(const mth::quatf& rotation);
		
		void setScale(const mth::vec3f& scale);
		void setLocalScale(const mth::vec3f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		mth::mat4f getWorldTransformMatrix() const;
		mth::mat4f getLocalTransformMatrix() const;
	};
}
