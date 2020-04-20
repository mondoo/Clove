#include "Bulb/ECS/Components/TransformComponent.hpp"

using namespace clv;

namespace blb::ecs{
	static void removeItemFromVector(const ComponentPtr<TransformComponent>& item, std::vector<ComponentPtr<TransformComponent>>& vector) {
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
		CLV_LOG_WARN("{0}: Unable to copy transform's children.", CLV_FUNCTION_NAME_PRETTY);

		if(parent != nullptr){
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
		CLV_LOG_WARN("{0}: Unable to copy transform's children.", CLV_FUNCTION_NAME_PRETTY);

		if(parent != nullptr){
			parent->children.push_back(this);
		}

		return *this;
	}

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent(){
		if(parent != nullptr){
			removeItemFromVector(this, parent->children);
		}

		for(auto child : children){
			child->parent.reset();
		}
	}

	clv::mth::vec3f TransformComponent::getPosition(TransformSpace space) const {
		switch(space) {
			case TransformSpace::Local:
				return getLocalPosition();
			case TransformSpace::World:
				return getWorldPosition();
			default:
				CLV_ASSERT(false, "Default statement hit in {0}", CLV_FUNCTION_NAME_PRETTY);
				return {};
		}
	}

	clv::mth::quatf TransformComponent::getRotation(TransformSpace space) const {
		switch(space) {
			case TransformSpace::Local:
				return getLocalRotation();
			case TransformSpace::World:
				return getWorldRotation();
			default:
				CLV_ASSERT(false, "Default statement hit in {0}", CLV_FUNCTION_NAME_PRETTY);
				return {};
		}
	}

	clv::mth::vec3f TransformComponent::getScale(TransformSpace space) const {
		switch(space) {
			case TransformSpace::Local:
				return getLocalScale();
			case TransformSpace::World:
				return getWorldScale();
			default:
				CLV_ASSERT(false, "Default statement hit in {0}", CLV_FUNCTION_NAME_PRETTY);
				return {};
		}
	}

	void TransformComponent::setPosition(const clv::mth::vec3f& position, TransformSpace space) {
		switch(space) {
			case TransformSpace::Local:
				setLocalPosition(position);
				break;
			case TransformSpace::World:
				setWorldPosition(position);
				break;
		}
	}

	void TransformComponent::setRotation(const clv::mth::quatf& rotation, TransformSpace space) {
		switch(space) {
			case TransformSpace::Local:
				setLocalRotation(rotation);
				break;
			case TransformSpace::World:
				setWorldRotation(rotation);
				break;
		}
	}

	void TransformComponent::setScale(const clv::mth::vec3f& scale, TransformSpace space) {
		switch(space) {
			case TransformSpace::Local:
				setLocalScale(scale);
				break;
			case TransformSpace::World:
				setWorldScale(scale);
				break;
		}
	}

	ComponentPtr<TransformComponent> TransformComponent::getParent() const {
		return parent;
	}

	void TransformComponent::addChild(ComponentPtr<TransformComponent> child) {
		if(child != nullptr && child != this){
			children.push_back(child);
			if(child->parent != nullptr){
				removeItemFromVector(child, child->parent->children);
			}
			child->parent = this;
		}
	}

	const std::vector<ComponentPtr<TransformComponent>>& TransformComponent::getChildren() const {
		return children;
	}
	
	mth::mat4f TransformComponent::getWorldTransformMatrix() const{
		if(parent != nullptr){
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

	mth::vec3f TransformComponent::getWorldPosition() const {
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		return { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
	}

	const mth::vec3f& TransformComponent::getLocalPosition() const {
		return localPosition;
	}

	mth::quatf TransformComponent::getWorldRotation() const {
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

	const mth::quatf& TransformComponent::getLocalRotation() const {
		return localRotation;
	}

	mth::vec3f TransformComponent::getWorldScale() const {
		const mth::mat4f transformMatrix = getWorldTransformMatrix();

		const mth::vec3f scaleX = { transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
		const mth::vec3f scaleY = { transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
		const mth::vec3f scaleZ = { transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

		return { mth::length(scaleX), mth::length(scaleY), mth::length(scaleZ) };
	}

	const mth::vec3f& TransformComponent::getLocalScale() const {
		return localScale;
	}

	void TransformComponent::setWorldPosition(const mth::vec3f& position) {
		if(parent != nullptr) {
			setLocalPosition(position - parent->getPosition());
		} else {
			setLocalPosition(position);
		}
	}

	void TransformComponent::setLocalPosition(const mth::vec3f& position) {
		localPosition = position;
	}

	void TransformComponent::setWorldRotation(const mth::quatf& rotation) {
		if(parent != nullptr) {
			const mth::mat4f rotMat			= mth::quaternionToMatrix4(rotation);
			const mth::mat4f parentRotMat	= mth::quaternionToMatrix4(parent->getRotation());
			const mth::mat4f adjustedRot	= parentRotMat / rotMat;

			setLocalRotation(mth::matrixToQuaternion(adjustedRot));
		} else {
			setLocalRotation(rotation);
		}
	}

	void TransformComponent::setLocalRotation(const mth::quatf& rotation) {
		localRotation = rotation;
	}

	void TransformComponent::setWorldScale(const mth::vec3f& scale) {
		if(parent != nullptr) {
			setLocalScale(scale / parent->getScale());
		} else {
			setLocalScale(scale);
		}
	}

	void TransformComponent::setLocalScale(const mth::vec3f& scale) {
		localScale = scale;
	}
}