#include "clvpch.hpp"
#include "TransformComponent.hpp"

namespace clv::ecs::d2{
	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent() = default;

	const math::Vector2f& TransformComponent::getPosition() const{
		return position;
	}

	const math::Vector2f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}

	float TransformComponent::getRotation() const{
		return rotation;
	}

	float TransformComponent::getLocalRotation() const{
		return localRotation;
	}

	const math::Vector2f& TransformComponent::getScale() const{
		return scale;
	}

	const math::Vector2f& TransformComponent::getLocalScale() const{
		return localScale;
	}

	void TransformComponent::setPosition(const math::Vector2f& position){
		desiredPosition = position;
	}

	void TransformComponent::setLocalPosition(const math::Vector2f& position){
		desiredLocalPosition = position;
	}

	void TransformComponent::setRotation(float rotation){
		desiredRotation = rotation;
	}

	void TransformComponent::setLocalRotation(float rotation){
		desiredLocalRotation = rotation;
	}

	void TransformComponent::setScale(const math::Vector2f& scale){
		desiredScale = scale;
	}

	void TransformComponent::setLocalScale(const math::Vector2f& scale){
		desiredLocalScale = scale;
	}

	TransformComponent* TransformComponent::getParent() const{
		return parent;
	}

	void TransformComponent::addChild(TransformComponent* child){
		if(child && child != this){
			children.push_back(child);
			child->parent = this;
		}
	}

	const math::Matrix4f& TransformComponent::getWorldTransformMatrix() const{
		return worldTransformMatrix;
	}

	const math::Matrix4f& TransformComponent::getLocalTransformMatrix() const{
		return localTransformMatrix;
	}
}