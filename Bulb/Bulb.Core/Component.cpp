#include "Component.hpp"

#include <Tunic/ECS/3D/Components/TransformComponent.hpp>

namespace Bulb::Core{
	TransformComponent::TransformComponent(tnc::ecs::_3D::TransformComponent* component)
		: component(component){
	}

	vec3^ TransformComponent::getPosition(){
		const clv::mth::vec3f pos = component->getPosition();
		return gcnew vec3{ pos.x, pos.y, pos.z };
	}

	void TransformComponent::setPosition(vec3^ position){
		component->setPosition({ position->x, position->y, position->z });
	}

	vec3^ TransformComponent::getRotation(){
		const clv::mth::vec3f rot = clv::mth::quaternionToEuler(component->getRotation());
		return gcnew vec3{ clv::mth::asDegrees(rot.x), clv::mth::asDegrees(rot.y), clv::mth::asDegrees(rot.z), };
	}

	void TransformComponent::setRotation(vec3^ rotation){
		const clv::mth::quatf rot = clv::mth::eulerToQuaternion(clv::mth::vec3f{ clv::mth::asRadians(rotation->x), clv::mth::asRadians(rotation->y), clv::mth::asRadians(rotation->z) });
		component->setRotation(rot);
	}

	vec3^ TransformComponent::getScale(){
		const clv::mth::vec3f scale = component->getScale();
		return gcnew vec3{ scale.x, scale.y, scale.z };
	}

	void TransformComponent::setScale(vec3^ scale){
		component->setScale({ scale->x, scale->y, scale->z });
	}
}