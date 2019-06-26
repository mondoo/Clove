#include "clvpch.hpp"
#include "Transform3DComponent.hpp"

namespace clv::ecs{
	Transform3DComponent::Transform3DComponent() = default;

	Transform3DComponent::Transform3DComponent(Transform3DComponent&& other) noexcept = default;

	Transform3DComponent& Transform3DComponent::operator=(Transform3DComponent&& other) noexcept = default;

	Transform3DComponent::~Transform3DComponent(){
		if(parent){
			auto removeIter = std::remove(parent->children.begin(), parent->children.end(), this);
			parent->children.erase(removeIter, parent->children.end());
		}

		for(auto* child : children){
			child->parent = nullptr;
		}
	}

	void Transform3DComponent::setLocalPosition(const math::Vector3f& inLocalPosition){
		localPosition = inLocalPosition;
	}

	const math::Vector3f& Transform3DComponent::getLocalPosition() const{
		return localPosition;
	}

	void Transform3DComponent::setLocalRotation(const std::pair<math::Vector3f, float>& inLocalRotation){
		localRotation = inLocalRotation;
	}

	const std::pair<math::Vector3f, float>& Transform3DComponent::getLocalRotation() const{
		return localRotation;
	}

	void Transform3DComponent::setLocalScale(const math::Vector3f& inLocalScale){
		localScale = inLocalScale;
	}

	const math::Vector3f& Transform3DComponent::getLocalScale() const{
		return localScale;
	}

	Transform3DComponent* Transform3DComponent::getParent() const{
		return parent;
	}

	void Transform3DComponent::addChild(Transform3DComponent* child){
		if(child && child != this){
			children.push_back(child);
			child->parent = this;
		}
	}
}