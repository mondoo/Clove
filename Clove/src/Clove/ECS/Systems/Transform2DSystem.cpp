#include "clvpch.hpp"
#include "Transform2DSystem.hpp"

namespace clv::ecs{
	Transform2DSystem::Transform2DSystem() = default;

	Transform2DSystem::Transform2DSystem(Transform2DSystem&& other) noexcept = default;

	Transform2DSystem& Transform2DSystem::operator=(Transform2DSystem&& other) noexcept = default;

	Transform2DSystem::~Transform2DSystem() = default;

	void Transform2DSystem::update(utl::DeltaTime deltaTime){
		//TODO: Remove all GLM calls

		for(auto& componentTuple : components){
			Transform2DComponent* transform = std::get<Transform2DComponent*>(componentTuple);
			Transform2DComponent* transformParent = transform->parent;

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
					transform->localRotation = getValueFromOptional(transform->desiredRotation) - transformParent->rotation;
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

			const math::Matrix4f localTranslationMatrix = math::translate(math::Matrix4f(1.0f), math::Vector3f{ transform->localPosition, 0.0f });
			const math::Matrix4f localRotationMatrix	= math::rotate(math::Matrix4f(1.0f), transform->localRotation, {0.0f, 0.0f, 1.0f});
			const math::Matrix4f localScaleMatrix		= math::scale(math::Matrix4f(1.0f), math::Vector3f{ transform->localScale, 0.0f });

			transform->localTransformMatrix = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

			if(transformParent){
				transform->worldTransformMatrix = transformParent->worldTransformMatrix * transform->localTransformMatrix;
			} else{
				transform->worldTransformMatrix = transform->localTransformMatrix;
			}

			auto [position, rotation, scale] = breakoutTransforms(transform->worldTransformMatrix);
			transform->position = position;
			transform->rotation = rotation;
			transform->scale = scale;
		}
	}

	math::Vector2f Transform2DSystem::getValueFromOptional(std::optional<math::Vector2f>& optional){
		math::Vector2f retVal;
		retVal = optional.value();
		optional.reset();
		return retVal;
	}

	float Transform2DSystem::getValueFromOptional(std::optional<float>& optional){
		float retVal;
		retVal = optional.value();
		optional.reset();
		return retVal;
	}

	std::tuple<math::Vector2f, float, math::Vector2f> Transform2DSystem::breakoutTransforms(math::Matrix4f transformMatrix){
		math::Vector2f position = { transformMatrix[3][0], transformMatrix[3][1] };

		transformMatrix[3][0] = 0.0f;
		transformMatrix[3][1] = 0.0f;
		transformMatrix[3][2] = 0.0f;

		const math::Vector3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const math::Vector3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const math::Vector3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		math::Vector2f scale = { glm::length(scaleX), glm::length(scaleY) };

		transformMatrix[0][0] /= scale.x;
		transformMatrix[0][1] /= scale.x;
		transformMatrix[0][2] /= scale.x;

		transformMatrix[1][0] /= scale.y;
		transformMatrix[1][1] /= scale.y;
		transformMatrix[1][2] /= scale.y;

		math::Quaternionf qrot = glm::toQuat(transformMatrix);

		float rotation = glm::eulerAngles(qrot).y;

		return { position, rotation, scale };
	}
}
