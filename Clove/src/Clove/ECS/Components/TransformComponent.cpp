#include "clvpch.hpp"
#include "TransformComponent.hpp"

namespace clv::ecs{
	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent() = default;

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
}