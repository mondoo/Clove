#include "TransformComponent.hpp"

namespace clv::ecs::_2D{
	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent() = default;

	math::Vector2f TransformComponent::getPosition() const{
		const math::Matrix4f transformMatrix = getWorldTransformMatrix();

		return { transformMatrix[3][0], transformMatrix[3][1] };
	}

	const math::Vector2f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}

	float TransformComponent::getRotation() const{
		math::Matrix4f transformMatrix = getWorldTransformMatrix();

		transformMatrix[3][0] = 0.0f;
		transformMatrix[3][1] = 0.0f;
		transformMatrix[3][2] = 0.0f;

		const math::Vector3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const math::Vector3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const math::Vector3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		math::Vector2f scale = { math::length(scaleX), math::length(scaleY) };

		transformMatrix[0][0] /= scale.x;
		transformMatrix[0][1] /= scale.x;
		transformMatrix[0][2] /= scale.x;

		transformMatrix[1][0] /= scale.y;
		transformMatrix[1][1] /= scale.y;
		transformMatrix[1][2] /= scale.y;

		math::Quaternionf qrot = math::matrixToQuaternion(transformMatrix);

		return math::quaternionToEuler(qrot).z;
	}

	float TransformComponent::getLocalRotation() const{
		return localRotation;
	}

	math::Vector2f TransformComponent::getScale() const{
		const math::Matrix4f transformMatrix = getWorldTransformMatrix();

		const math::Vector3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const math::Vector3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const math::Vector3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		return { math::length(scaleX), math::length(scaleY) };
	}

	const math::Vector2f& TransformComponent::getLocalScale() const{
		return localScale;
	}

	void TransformComponent::setPosition(const math::Vector2f& position){
		if(parent){
			setLocalPosition(position - parent->getPosition());
		} else{
			setLocalPosition(position);
		}
	}

	void TransformComponent::setLocalPosition(const math::Vector2f& position){
		localPosition = position;
	}

	void TransformComponent::setRotation(float rotation){
		if(parent){
			setLocalRotation(rotation - parent->getRotation());
		} else{
			setLocalRotation(rotation);
		}
	}

	void TransformComponent::setLocalRotation(float rotation){
		localRotation = rotation;
	}

	void TransformComponent::setScale(const math::Vector2f& scale){
		if(parent){
			setLocalScale(scale / parent->getScale());
		} else{
			setLocalScale(scale);
		}
	}

	void TransformComponent::setLocalScale(const math::Vector2f& scale){
		localScale = scale;
	}

	TransformComponent* TransformComponent::getParent() const{
		return parent;
	}

	void TransformComponent::addChild(TransformComponent* child){
		if(child && child != this){
			children.push_back(child);
			child->parent = this;
		}
	}

	math::Matrix4f TransformComponent::getWorldTransformMatrix() const{
		if(parent){
			return parent->getWorldTransformMatrix() * getLocalTransformMatrix();
		} else{
			return getLocalTransformMatrix();
		}
	}

	math::Matrix4f TransformComponent::getLocalTransformMatrix() const{
		const math::Matrix4f translationMatrix	= math::translate(math::Matrix4f(1.0f), math::Vector3f{ localPosition, 0.0f });
		const math::Matrix4f rotationMatrix		= math::rotate(math::Matrix4f(1.0f), localRotation, { 0.0f, 0.0f, 1.0f });
		const math::Matrix4f scaleMatrix		= math::scale(math::Matrix4f(1.0f), math::Vector3f{ localScale, 0.0f });

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
}