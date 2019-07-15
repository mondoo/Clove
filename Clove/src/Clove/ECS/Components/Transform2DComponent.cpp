#include "clvpch.hpp"
#include "Transform2DComponent.hpp"

namespace clv::ecs{
	Transform2DComponent::Transform2DComponent() = default;

	Transform2DComponent::Transform2DComponent(Transform2DComponent&& other) noexcept = default;

	Transform2DComponent& Transform2DComponent::operator=(Transform2DComponent&& other) noexcept = default;

	Transform2DComponent::~Transform2DComponent() = default;

	void Transform2DComponent::setLocalPosition(const math::Vector2f& inLocalPosition){
		localPosition = inLocalPosition;
	}

	const math::Vector2f& Transform2DComponent::getLocalPosition() const{
		return localPosition;
	}

	void Transform2DComponent::setLocalRotation(const std::pair<math::Vector2f, float>& inLocalRotation){
		localRotation = inLocalRotation;
	}

	const std::pair<math::Vector2f, float>& Transform2DComponent::getLocalRotation() const{
		return localRotation;
	}

	void Transform2DComponent::setLocalScale(const math::Vector2f& inLocalScale){
		localScale = inLocalScale;
	}

	const math::Vector2f& Transform2DComponent::getLocalScale() const{
		return localScale;
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
}