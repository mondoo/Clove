#include "Tunic/ECS/3D/Components/TransformComponent.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	static void removeItemFromVector(TransformComponent* item, std::vector<TransformComponent*>& vector){
		auto removeIter = std::remove(vector.begin(), vector.end(), item);
		vector.erase(removeIter, vector.end());
	}

	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(const TransformComponent& other){
		localPosition = other.localPosition;
		localRotation = other.localRotation;
		localScale = other.localScale;

		parent = other.parent;
		//Not copying children for now, transform component has no knowledge of other components

		if(parent){
			parent->children.push_back(this);
		}
	}

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(const TransformComponent& other){
		localPosition = other.localPosition;
		localRotation = other.localRotation;
		localScale = other.localScale;

		parent = other.parent;
		//Not copying children for now, transform component has no knowledge of other components

		if(parent){
			parent->children.push_back(this);
		}

		return *this;
	}

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent(){
		if(parent){
			removeItemFromVector(this, parent->children);
		}

		for(auto* child : children){
			child->parent = nullptr;
		}
	}

	mth::vec3f TransformComponent::getPosition() const{
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		return { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
	}

	const mth::vec3f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}
	
	mth::quatf TransformComponent::getRotation() const{
		mth::mat4f transformMatrix = getWorldTransformMatrix();
		
		transformMatrix[3][0] = 0.0f;
		transformMatrix[3][1] = 0.0f;
		transformMatrix[3][2] = 0.0f;

		const mth::vec3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const mth::vec3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const mth::vec3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		mth::vec3f scale = { mth::length(scaleX), mth::length(scaleY), mth::length(scaleZ) };

		transformMatrix[0][0] /= scale.x;
		transformMatrix[0][1] /= scale.x;
		transformMatrix[0][2] /= scale.x;

		transformMatrix[1][0] /= scale.y;
		transformMatrix[1][1] /= scale.y;
		transformMatrix[1][2] /= scale.y;

		transformMatrix[2][0] /= scale.z;
		transformMatrix[2][1] /= scale.z;
		transformMatrix[2][2] /= scale.z;

		return mth::matrixToQuaternion(transformMatrix);
	}

	const mth::quatf& TransformComponent::getLocalRotation() const{
		return localRotation;
	}
	
	mth::vec3f TransformComponent::getScale() const{
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		const mth::vec3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const mth::vec3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const mth::vec3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		return { mth::length(scaleX), mth::length(scaleY), mth::length(scaleZ) };
	}

	const mth::vec3f& TransformComponent::getLocalScale() const{
		return localScale;
	}
	
	void TransformComponent::setPosition(const mth::vec3f& position){
		if(parent){
			setLocalPosition(position - parent->getPosition());
		} else{
			setLocalPosition(position);
		}
	}

	void TransformComponent::setLocalPosition(const mth::vec3f& position){
		localPosition = position;
	}

	void TransformComponent::setRotation(const mth::quatf& rotation){
		if(parent){
			const mth::mat4f rotMat			= mth::quaternionToMatrix4(rotation);
			const mth::mat4f parentRotMat	= mth::quaternionToMatrix4(parent->getRotation());
			const mth::mat4f adjustedRot	= parentRotMat / rotMat;

			setLocalRotation(mth::matrixToQuaternion(adjustedRot));
		} else{
			setLocalRotation(rotation);
		}
	}

	void TransformComponent::setLocalRotation(const mth::quatf& rotation){
		localRotation = rotation;
	}

	void TransformComponent::setScale(const mth::vec3f& scale){
		if(parent){
			setLocalScale(scale / parent->getScale());
		} else{
			setLocalScale(scale);
		}
	}

	void TransformComponent::setLocalScale(const mth::vec3f& scale){
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
	
	mth::mat4f TransformComponent::getWorldTransformMatrix() const{
		if(parent){
			return parent->getWorldTransformMatrix() * getLocalTransformMatrix();
		} else{
			return getLocalTransformMatrix();
		}
	}

	mth::mat4f TransformComponent::getLocalTransformMatrix() const{
		const mth::mat4f translationMatrix	= mth::translate(mth::mat4f(1.0f), localPosition);
		const mth::mat4f rotationMatrix		= mth::quaternionToMatrix4(localRotation);
		const mth::mat4f scaleMatrix		= mth::scale(mth::mat4f(1.0f), localScale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
}