#include "clvpch.hpp"
#include "Transform3DSystem.hpp"

namespace clv::ecs{
	Transform3DSystem::Transform3DSystem() = default;

	Transform3DSystem::Transform3DSystem(Transform3DSystem&& other) noexcept = default;

	Transform3DSystem& Transform3DSystem::operator=(Transform3DSystem&& other) noexcept = default;

	Transform3DSystem::~Transform3DSystem() = default;

	void Transform3DSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			transform->worldTransformMatrix = getTransformWorldMatrix(transform);
		}
	}

	math::Matrix4f Transform3DSystem::getTransformWorldMatrix(Transform3DComponent* component){
		const auto& [rot, angle] = component->getLocalRotation();

		math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), component->getLocalPosition());
		math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
		math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), component->getLocalScale());

		math::Matrix4f transform = translation * rotation * scale;

		if(Transform3DComponent* parent = component->getParent()){
			return getTransformWorldMatrix(parent) * transform;
		} else{
			return transform;
		}
	}
}