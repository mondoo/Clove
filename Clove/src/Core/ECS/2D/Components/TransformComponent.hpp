#pragma once

#include "Core/ECS/Component.hpp"

namespace clv::ecs::_2D{
	class TransformComponent : public Component<_2D::TransformComponent>{
		friend class TransformSystem;

		//VARIABLES
	private:
		mth::vec2f	localPosition	= { 0.0f, 0.0f };
		float			localRotation	= 0.0f;
		mth::vec2f	localScale		= { 1.0f, 1.0f };

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

		mth::vec2f getPosition() const;
		const mth::vec2f& getLocalPosition() const;
		
		float getRotation() const;
		float getLocalRotation() const;
		
		mth::vec2f getScale() const;
		const mth::vec2f& getLocalScale() const;
		
		void setPosition(const mth::vec2f& position);
		void setLocalPosition(const mth::vec2f& position);

		void setRotation(float rotation);
		void setLocalRotation(float inLocalRotation);

		void setScale(const mth::vec2f& scale);
		void setLocalScale(const mth::vec2f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		mth::mat4f getWorldTransformMatrix() const;
		mth::mat4f getLocalTransformMatrix() const;
	};
}
