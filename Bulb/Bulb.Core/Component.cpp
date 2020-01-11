#include "Component.hpp"

#include <Tunic/ECS/3D/Components/TransformComponent.hpp>

namespace Bulb::Core{
	TransformComponent::TransformComponent(tnc::ecs::_3D::TransformComponent* component)
		: component(component){
	}

	float TransformComponent::getX(){
		return component->getPosition().x;
	}

	void TransformComponent::setX(float val){
		const auto pos = component->getPosition();
		component->setPosition({ val, pos.y, pos.z });
	}

	float TransformComponent::getY(){
		return component->getPosition().y;
	}

	void TransformComponent::setY(float val){
		const auto pos = component->getPosition();
		component->setPosition({ pos.x, val, pos.z });
	}

	float TransformComponent::getZ(){
		return component->getPosition().z;
	}

	void TransformComponent::setZ(float val){
		const auto pos = component->getPosition();
		component->setPosition({ pos.x, pos.y, val });
	}
}