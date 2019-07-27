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

			//Position
			if(transform->parent){
				if(transform->desiredLocalPosition){
					transform->localPosition = transform->desiredLocalPosition.value();

					transform->desiredLocalPosition.reset();
				} else if(transform->desiredPosition){
					transform->localPosition = transform->desiredPosition.value() - transform->parent->position;

					transform->desiredPosition.reset();
				}
					
				transform->position = transform->localPosition + transform->parent->position;
			} else{
				if(transform->desiredLocalPosition){
					transform->localPosition	= transform->desiredLocalPosition.value();
					transform->position			= transform->localPosition;
					
					transform->desiredLocalPosition.reset();
				} else if(transform->desiredPosition){
					transform->position = transform->desiredPosition.value();

					transform->desiredPosition.reset();
				}
			}

			//Rotation
			if(transform->parent){
				//This will have to do some shit with rotating around the parent etc.
				//Adjusting the position
			} else{
				//Just rotate it in world space - easy
			}

			//Scale
			if(transform->parent){
				
			} else{
			
			}
		}
	}
}