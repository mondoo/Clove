#include "TransformComponent.hpp"

namespace clv::ecs::d3{
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

	const math::Vector3f& TransformComponent::getPosition() const{
		return position;
	}

	const math::Vector3f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}
	
	const math::Quaternionf& TransformComponent::getRotation() const{
		return rotation;
	}

	const math::Quaternionf& TransformComponent::getLocalRotation() const{
		return localRotation;
	}
	
	const math::Vector3f& TransformComponent::getScale() const{
		return scale;
	}

	const math::Vector3f& TransformComponent::getLocalScale() const{
		return localScale;
	}
	
	void TransformComponent::setPosition(const math::Vector3f& position){
		desiredPosition = position;
	}

	void TransformComponent::setLocalPosition(const math::Vector3f& position){
		desiredLocalPosition = position;
	}

	void TransformComponent::setRotation(const math::Quaternionf& rotation){
		desiredRotation = rotation;
	}

	void TransformComponent::setLocalRotation(const math::Quaternionf& rotation){
		desiredLocalRotation = rotation;
	}

	void TransformComponent::setScale(const math::Vector3f& scale){
		desiredScale = scale;
	}

	void TransformComponent::setLocalScale(const math::Vector3f& scale){
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