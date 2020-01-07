#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::ecs::ui{
	class TransformComponent : public Component<ui::TransformComponent>{
		//VARIABLES
	private:
		clv::mth::vec2f	localPosition = { 0.0f, 0.0f };
		float			localRotation = 0.0f;
		clv::mth::vec2f	localScale = { 1.0f, 1.0f };

		clv::mth::vec2f alignment = { 0.0f, 0.0f };
		clv::mth::vec2f anchor = { 0.0f, 0.0f };

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

		const clv::mth::vec2f& getAlignment() const;

		const clv::mth::vec2f& getAnchor() const;

		void setPosition(const clv::mth::vec2f& position);
		void setLocalPosition(const clv::mth::vec2f& position);

		void setRotation(float rotation);
		void setLocalRotation(float inLocalRotation);

		void setScale(const clv::mth::vec2f& scale);
		void setLocalScale(const clv::mth::vec2f& scale);

		void setAlignment(const clv::mth::vec2f& alignment);

		void setAnchor(const clv::mth::vec2f& anchor);

		TransformComponent* getParent() const;
		void addChild(TransformComponent* child);

		clv::mth::mat4f getWorldTransformMatrix() const;
		clv::mth::mat4f getLocalTransformMatrix() const;
	};
}