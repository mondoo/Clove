#include "Component.hpp"

#include <Tunic/ECS/3D/Components/TransformComponent.hpp>

namespace Bulb::Core{
	TransformComponent::TransformComponent(tnc::ecs::_3D::TransformComponent* component)
		: component(component){
	}

	vec3^ TransformComponent::getPosition(){
		clv::mth::vec3f pos = component->getPosition();
		vec3^ retPos = gcnew vec3{ pos.x, pos.y, pos.z };
		return retPos;
	}

	void TransformComponent::setPosition(vec3^ position){
		clv::mth::vec3f pos = { position->x, position->y, position->z };
		component->setPosition(pos);
	}
}