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

			math::Matrix4f localTranslationMatrix = math::Matrix4f(1.0f);
			math::Matrix4f localRotationMatrix = math::Matrix4f(1.0f);
			math::Matrix4f localScaleMatrix = math::Matrix4f(1.0f);

			//Position
			if(transform->parent){
				if(transform->desiredLocalPosition){
					transform->localPosition = transform->desiredLocalPosition.value();

					transform->desiredLocalPosition.reset();
				} else if(transform->desiredPosition){
					transform->localPosition = transform->desiredPosition.value() - transform->parent->position;

					transform->desiredPosition.reset();
				}
			} else{
				if(transform->desiredLocalPosition){
					transform->localPosition = transform->desiredLocalPosition.value();

					transform->desiredLocalPosition.reset();
				} else if(transform->desiredPosition){
					transform->localPosition = transform->desiredPosition.value();

					transform->desiredPosition.reset();
				}
			}

			localTranslationMatrix = math::translate(math::Matrix4f(1.0f), transform->localPosition);

			//Rotation
			if(transform->parent){
				if(transform->desiredLocalRotation){
					transform->localRotation = transform->desiredLocalRotation.value();

					transform->desiredLocalRotation.reset();
				} else if(transform->desiredRotation){
					//TODO: I think this is right
					const auto desiredRot = math::quaternionToMatrix4(transform->desiredRotation.value());
					const auto parentRot = math::quaternionToMatrix4(transform->parent->rotation);
					const auto adjustedRot = parentRot / desiredRot;

					transform->localRotation = glm::toQuat(adjustedRot);;

					transform->desiredRotation.reset();
				}

			} else{
				if(transform->desiredLocalRotation){
					transform->localRotation = transform->desiredLocalRotation.value();

					transform->desiredLocalRotation.reset();
				} else if(transform->desiredRotation){
					transform->localRotation = transform->desiredRotation.value();

					transform->desiredRotation.reset();
				}
			}

			localRotationMatrix = math::quaternionToMatrix4(transform->localRotation);

			//Scale
			if(transform->parent){
				if(transform->desiredLocalScale){
					transform->localScale = transform->desiredLocalScale.value();

					transform->desiredLocalScale.reset();
				} else if(transform->desiredScale){
					transform->localScale = transform->desiredScale.value() / transform->parent->scale;

					transform->desiredScale.reset();
				}
			} else{
				if(transform->desiredLocalScale){
					transform->localScale = transform->desiredLocalScale.value();

					transform->desiredLocalScale.reset();
				} else if(transform->desiredScale){
					transform->localScale = transform->desiredScale.value();

					transform->desiredScale.reset();
				}
			}

			localScaleMatrix = math::scale(math::Matrix4f(1.0f), transform->localScale);

			transform->localTransformMatrix = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

			if(transform->parent){
				transform->worldTransformMatrix = transform->parent->worldTransformMatrix * transform->localTransformMatrix;
			} else{
				transform->worldTransformMatrix = transform->localTransformMatrix;
			}

			math::Matrix4f worldCopy = transform->worldTransformMatrix;

			//Get world stuff
			transform->position = { worldCopy[3][0], worldCopy[3][1], worldCopy[3][2] };
			worldCopy[3][0] = 0.0f;
			worldCopy[3][1] = 0.0f;
			worldCopy[3][2] = 0.0f;

			const math::Vector3f scaleX = { worldCopy[0][0], worldCopy[0][1], worldCopy[0][2] };
			const math::Vector3f scaleY = { worldCopy[1][0], worldCopy[1][1], worldCopy[1][2] };
			const math::Vector3f scaleZ = { worldCopy[2][0], worldCopy[2][1], worldCopy[2][2] };
			transform->scale = { glm::length(scaleX), glm::length(scaleY), glm::length(scaleZ) };

			worldCopy[0][0] /= transform->scale.x;
			worldCopy[0][1] /= transform->scale.x;
			worldCopy[0][2] /= transform->scale.x;

			worldCopy[1][0] /= transform->scale.y;
			worldCopy[1][1] /= transform->scale.y;
			worldCopy[1][2] /= transform->scale.y;

			worldCopy[2][0] /= transform->scale.z;
			worldCopy[2][1] /= transform->scale.z;
			worldCopy[2][2] /= transform->scale.z;

			transform->rotation = glm::toQuat(worldCopy);
		}
	}
}