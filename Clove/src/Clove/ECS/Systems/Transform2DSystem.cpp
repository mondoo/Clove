#include "clvpch.hpp"
#include "Transform2DSystem.hpp"

namespace clv::ecs{
	Transform2DSystem::Transform2DSystem() = default;

	Transform2DSystem::Transform2DSystem(Transform2DSystem&& other) noexcept = default;

	Transform2DSystem& Transform2DSystem::operator=(Transform2DSystem&& other) noexcept = default;

	Transform2DSystem::~Transform2DSystem() = default;

	void Transform2DSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform2DComponent* transform = std::get<Transform2DComponent*>(componentTuple);
			//transform->worldTransformMatrix = getTransformWorldMatrix(transform);
		}
	}

	/*math::Matrix4f Transform2DSystem::getTransformWorldMatrix(Transform2DComponent* component){
		const auto& [rot, angle] = component->getLocalRotation();

		math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), math::Vector3f(component->getLocalPosition(), 0.0f));
		math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, math::Vector3f(rot, 0.0f));
		math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), math::Vector3f(component->getLocalScale(), 0.0f));

		math::Matrix4f transform = translation * rotation * scale;

		if(Transform2DComponent * parent = component->getParent()){
			return getTransformWorldMatrix(parent) * transform;
		} else{
			return transform;
		}
	}*/
}
