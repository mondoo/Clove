#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::ecs::_3D{
	class TransformComponent : public Component<_3D::TransformComponent>{
		//VARIABLES
	private:
		clv::mth::vec3f	localPosition	= { 0.0f, 0.0f, 0.0f };
		clv::mth::quatf	localRotation	= mth::quatf({ 0.0f, 0.0f, 0.0f });
		clv::mth::vec3f	localScale		= { 1.0f, 1.0f, 1.0f };

		TransformComponent* parent = nullptr;
		std::vector<TransformComponent*> children;

		//FUNCTIONS
	public:
		TransformComponent();

		TransformComponent(const TransformComponent& other);
		TransformComponent(TransformComponent&& other) noexcept;

		TransformComponent& operator=(const TransformComponent& other);
		TransformComponent& operator=(TransformComponent&& other) noexcept;

		virtual ~TransformComponent();

		clv::mth::vec3f getPosition() const;
		const clv::mth::vec3f& getLocalPosition() const;

		clv::mth::quatf getRotation() const;
		const clv::mth::quatf& getLocalRotation() const;

		clv::mth::vec3f getScale() const;
		const clv::mth::vec3f& getLocalScale() const;

		void setPosition(const clv::mth::vec3f& position);
		void setLocalPosition(const clv::mth::vec3f& position);
		
		void setRotation(const clv::mth::quatf& rotation);
		void setLocalRotation(const clv::mth::quatf& rotation);
		
		void setScale(const clv::mth::vec3f& scale);
		void setLocalScale(const clv::mth::vec3f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		clv::mth::mat4f getWorldTransformMatrix() const;
		clv::mth::mat4f getLocalTransformMatrix() const;
	};
}
