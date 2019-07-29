#include "clvpch.hpp"
#include "Transform2DComponent.hpp"

namespace clv::ecs{
	Transform2DComponent::Transform2DComponent() = default;

	Transform2DComponent::Transform2DComponent(Transform2DComponent&& other) noexcept = default;

	Transform2DComponent& Transform2DComponent::operator=(Transform2DComponent&& other) noexcept = default;

	Transform2DComponent::~Transform2DComponent() = default;

	const math::Vector2f& Transform2DComponent::getPosition() const{
		return position;
	}

	const math::Vector2f& Transform2DComponent::getLocalPosition() const{
		return localPosition;
	}

	float Transform2DComponent::getRotation() const{
		return rotation;
	}

	float Transform2DComponent::getLocalRotation() const{
		return localRotation;
	}

	const math::Vector2f& Transform2DComponent::getScale() const{
		return scale;
	}

	const math::Vector2f& Transform2DComponent::getLocalScale() const{
		return localScale;
	}

	void Transform2DComponent::setPosition(const math::Vector2f& position){
		desiredPosition = position;
	}

	void Transform2DComponent::setLocalPosition(const math::Vector2f& position){
		desiredLocalPosition = position;
	}

	void Transform2DComponent::setRotation(float rotation){
		desiredRotation = rotation;
	}

	void Transform2DComponent::setLocalRotation(float rotation){
		desiredLocalRotation = rotation;
	}

	void Transform2DComponent::setScale(const math::Vector2f& scale){
		desiredScale = scale;
	}

	void Transform2DComponent::setLocalScale(const math::Vector2f& scale){
		desiredLocalScale = scale;
	}

	Transform2DComponent* Transform2DComponent::getParent() const{
		return parent;
	}

	void Transform2DComponent::addChild(Transform2DComponent* child){
		if(child && child != this){
			children.push_back(child);
			child->parent = this;
		}
	}

	const math::Matrix3f& Transform2DComponent::getWorldTransformMatrix() const{
		return worldTransformMatrix;
	}

	const math::Matrix3f& Transform2DComponent::getLocalTransformMatrix() const{
		return localTransformMatrix;
	}
}