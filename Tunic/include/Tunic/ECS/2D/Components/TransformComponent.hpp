#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::ecs::_2D{
	class TransformComponent : public Component<_2D::TransformComponent>{
		//VARIABLES
	private:
		clv::mth::vec2f	localPosition	= { 0.0f, 0.0f };
		float		localRotation	= 0.0f;
		clv::mth::vec2f	localScale		= { 1.0f, 1.0f };

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

		clv::mth::vec2f getPosition() const;
		const clv::mth::vec2f& getLocalPosition() const;
		
		float getRotation() const;
		float getLocalRotation() const;
		
		clv::mth::vec2f getScale() const;
		const clv::mth::vec2f& getLocalScale() const;
		
		void setPosition(const clv::mth::vec2f& position);
		void setLocalPosition(const clv::mth::vec2f& position);

		void setRotation(float rotation);
		void setLocalRotation(float inLocalRotation);

		void setScale(const clv::mth::vec2f& scale);
		void setLocalScale(const clv::mth::vec2f& scale);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		clv::mth::mat4f getWorldTransformMatrix() const;
		clv::mth::mat4f getLocalTransformMatrix() const;
	};
}
