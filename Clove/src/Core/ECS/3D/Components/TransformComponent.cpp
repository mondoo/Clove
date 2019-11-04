#include "TransformComponent.hpp"

namespace clv::ecs::_3D{
	static void removeItemFromVector(TransformComponent* item, std::vector<TransformComponent*>& vector){
		auto removeIter = std::remove(vector.begin(), vector.end(), item);
		vector.erase(removeIter, vector.end());
	}

	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent(){
		if(parent){
			removeItemFromVector(this, parent->children);
		}

		for(auto* child : children){
			child->parent = nullptr;
		}
	}

	math::Vector3f TransformComponent::getPosition() const{
		const math::Matrix4f transformMatrix = getWorldTransformMatrix();

		return { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
	}

	const math::Vector3f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}
	
	math::Quaternionf TransformComponent::getRotation() const{
		math::Matrix4f transformMatrix = getWorldTransformMatrix();
		
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

		return math::matrixToQuaternion(transformMatrix);
	}

	const math::Quaternionf& TransformComponent::getLocalRotation() const{
		return localRotation;
	}
	
	math::Vector3f TransformComponent::getScale() const{
		const math::Matrix4f transformMatrix = getWorldTransformMatrix();

		const math::Vector3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const math::Vector3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const math::Vector3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		return { math::length(scaleX), math::length(scaleY), math::length(scaleZ) };
	}

	const math::Vector3f& TransformComponent::getLocalScale() const{
		return localScale;
	}
	
	void TransformComponent::setPosition(const math::Vector3f& position){
		if(parent){
			setLocalPosition(position - parent->getPosition());
		} else{
			setLocalPosition(position);
		}
	}

	void TransformComponent::setLocalPosition(const math::Vector3f& position){
		localPosition = position;
	}

	void TransformComponent::setRotation(const math::Quaternionf& rotation){
		if(parent){
			const math::Matrix4f rotMat			= math::quaternionToMatrix4(rotation);
			const math::Matrix4f parentRotMat	= math::quaternionToMatrix4(parent->getRotation());
			const math::Matrix4f adjustedRot	= parentRotMat / rotMat;

			setLocalRotation(math::matrixToQuaternion(adjustedRot));
		} else{
			setLocalRotation(rotation);
		}
	}

	void TransformComponent::setLocalRotation(const math::Quaternionf& rotation){
		localRotation = rotation;
	}

	void TransformComponent::setScale(const math::Vector3f& scale){
		if(parent){
			setLocalScale(scale / parent->getScale());
		} else{
			setLocalScale(scale);
		}
	}

	void TransformComponent::setLocalScale(const math::Vector3f& scale){
		localScale = scale;
	}

	TransformComponent* TransformComponent::getParent() const{
		return parent;
	}

	void TransformComponent::addChild(TransformComponent* child){
		if(child && child != this){
			children.push_back(child);
			if(child->parent){
				removeItemFromVector(child, child->parent->children);
			}
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
		const math::Matrix4f translationMatrix	= math::translate(math::Matrix4f(1.0f), localPosition);
		const math::Matrix4f rotationMatrix		= math::quaternionToMatrix4(localRotation);
		const math::Matrix4f scaleMatrix		= math::scale(math::Matrix4f(1.0f), localScale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
}