#include "clvpch.hpp"
#include "TransformComponent.hpp"

namespace clv::ecs{
	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent(){
		if(parent){
			auto removeIter = std::remove(parent->children.begin(), parent->children.end(), this);
			parent->children.erase(removeIter, parent->children.end());
		}

		for(auto* child : children){
			child->parent = nullptr;
		}
	}

	void TransformComponent::setLocalPosition(const math::Vector3f& inLocalPosition){
		localPosition = inLocalPosition;
	}

	const math::Vector3f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}

	void TransformComponent::setLocalRotation(const std::pair<math::Vector3f, float>& inLocalRotation){
		localRotation = inLocalRotation;
	}

	const std::pair<math::Vector3f, float>& TransformComponent::getLocalRotation() const{
		return localRotation;
	}

	void TransformComponent::setLocalScale(const math::Vector3f& inLocalScale){
		localScale = inLocalScale;
	}

	const math::Vector3f& TransformComponent::getLocalScale() const{
		return localScale;
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
}