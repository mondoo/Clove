#include "Tunic/ECS/2D/Components/TransformComponent.hpp"

using namespace clv;

namespace tnc::ecs::_2D{
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

	mth::vec2f TransformComponent::getPosition() const{
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		return { transformMatrix[3][0], transformMatrix[3][1] };
	}

	const mth::vec2f& TransformComponent::getLocalPosition() const{
		return localPosition;
	}

	float TransformComponent::getRotation() const{
		mth::mat4f transformMatrix = getWorldTransformMatrix();

		transformMatrix[3][0] = 0.0f;
		transformMatrix[3][1] = 0.0f;

		const mth::vec3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const mth::vec3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };

		mth::vec2f scale = { mth::length(scaleX), mth::length(scaleY) };

		transformMatrix[0][0] /= scale.x;
		transformMatrix[0][1] /= scale.x;
		transformMatrix[0][2] /= scale.x;

		transformMatrix[1][0] /= scale.y;
		transformMatrix[1][1] /= scale.y;
		transformMatrix[1][2] /= scale.y;

		const mth::vec3f euler = mth::matrixToEuler(transformMatrix);

		return euler.z;
	}

	float TransformComponent::getLocalRotation() const{
		return localRotation;
	}

	mth::vec2f TransformComponent::getScale() const{
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		const mth::vec3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const mth::vec3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };

		return { mth::length(scaleX), mth::length(scaleY) };
	}

	const mth::vec2f& TransformComponent::getLocalScale() const{
		return localScale;
	}

	void TransformComponent::setPosition(const mth::vec2f& position){
		if(parent){
			setLocalPosition(position - parent->getPosition());
		} else{
			setLocalPosition(position);
		}
	}

	void TransformComponent::setLocalPosition(const mth::vec2f& position){
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

	void TransformComponent::setScale(const mth::vec2f& scale){
		if(parent){
			setLocalScale(scale / parent->getScale());
		} else{
			setLocalScale(scale);
		}
	}

	void TransformComponent::setLocalScale(const mth::vec2f& scale){
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
		const mth::mat4f translationMatrix	= mth::translate(mth::mat4f(1.0f), mth::vec3f{ localPosition, 0.0f });
		const mth::mat4f rotationMatrix		= mth::rotate(mth::mat4f(1.0f), localRotation, { 0.0f, 0.0f, 1.0f });
		const mth::mat4f scaleMatrix		= mth::scale(mth::mat4f(1.0f), mth::vec3f{ localScale, 0.0f });

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
}