#include "TransformSystem.hpp"

namespace clv::ecs::_3D{
	TransformSystem::TransformSystem() = default;

	TransformSystem::TransformSystem(TransformSystem&& other) noexcept = default;

	TransformSystem& TransformSystem::operator=(TransformSystem&& other) noexcept = default;

	TransformSystem::~TransformSystem() = default;

	void TransformSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			TransformComponent* transformParent = transform->parent;

			//Position
			if(transform->desiredLocalPosition){
				transform->localPosition = getValueFromOptional(transform->desiredLocalPosition);
			} else if(transform->desiredPosition){
				if(transformParent){
					transform->localPosition = getValueFromOptional(transform->desiredPosition) - transformParent->position;
				} else{
					transform->localPosition = getValueFromOptional(transform->desiredPosition);
				}
			}

			//Rotation
			if(transform->desiredLocalRotation){
				transform->localRotation = getValueFromOptional(transform->desiredLocalRotation);
			} else if(transform->desiredRotation){
				if(transformParent){
					const math::Matrix4f desiredRot = math::quaternionToMatrix4(getValueFromOptional(transform->desiredRotation));
					const math::Matrix4f parentRot = math::quaternionToMatrix4(transformParent->rotation);
					const math::Matrix4f adjustedRot = parentRot / desiredRot;

					transform->localRotation = math::matrixToQuaternion(adjustedRot);
				} else{
					transform->localRotation = getValueFromOptional(transform->desiredRotation);
				}
			}

			//Scale
			if(transform->desiredLocalScale){
				transform->localScale = getValueFromOptional(transform->desiredLocalScale);
			} else if(transform->desiredScale){
				if(transformParent){
					transform->localScale = getValueFromOptional(transform->desiredScale) / transformParent->scale;
				} else{
					transform->localScale = getValueFromOptional(transform->desiredScale);
				}
			}

			const math::Matrix4f localTranslationMatrix = math::translate(math::Matrix4f(1.0f), transform->localPosition);
			const math::Matrix4f localRotationMatrix	= math::quaternionToMatrix4(transform->localRotation);
			const math::Matrix4f localScaleMatrix		= math::scale(math::Matrix4f(1.0f), transform->localScale);

			transform->localTransformMatrix = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

			if(transformParent){
				transform->worldTransformMatrix = transformParent->worldTransformMatrix * transform->localTransformMatrix;
			} else{
				transform->worldTransformMatrix = transform->localTransformMatrix;
			}

			auto [position, rotation, scale] = breakoutTransforms(transform->worldTransformMatrix);
			transform->position = position;
			transform->rotation = rotation;
			transform->scale	= scale;
		}
	}

	math::Vector3f TransformSystem::getValueFromOptional(std::optional<math::Vector3f>& optional){
		math::Vector3f retVal;
		retVal = optional.value();
		optional.reset();
		return retVal;
	}

	math::Quaternionf TransformSystem::getValueFromOptional(std::optional<math::Quaternionf>& optional){
		math::Quaternionf retVal;
		retVal = optional.value();
		optional.reset();
		return retVal;
	}

	std::tuple<math::Vector3f, math::Quaternionf, math::Vector3f> TransformSystem::breakoutTransforms(math::Matrix4f transformMatrix){
		math::Vector3f position = { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };

		transformMatrix[3][0] = 0.0f;
		transformMatrix[3][1] = 0.0f;
		transformMatrix[3][2] = 0.0f;

		const math::Vector3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const math::Vector3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const math::Vector3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		math::Vector3f scale = { math::length(scaleX), math::length(scaleY), math::length(scaleZ) };

		transformMatrix[0][0] /= scale.x;
		transformMatrix[0][1] /= scale.x;
		transformMatrix[0][2] /= scale.x;

		transformMatrix[1][0] /= scale.y;
		transformMatrix[1][1] /= scale.y;
		transformMatrix[1][2] /= scale.y;

		transformMatrix[2][0] /= scale.z;
		transformMatrix[2][1] /= scale.z;
		transformMatrix[2][2] /= scale.z;

		math::Quaternionf rotation = math::matrixToQuaternion(transformMatrix);

		return { position, rotation, scale };
	}
}