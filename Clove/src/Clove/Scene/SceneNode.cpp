#include "clvpch.hpp"
#include "SceneNode.hpp"

namespace clv{
	namespace scene{
		SceneNode::SceneNode() = default;

		SceneNode::SceneNode(const SceneNode& other) = default;

		SceneNode::SceneNode(SceneNode&& other) noexcept = default;

		SceneNode::~SceneNode() = default;

		void SceneNode::update(float deltaSeconds){
			for(auto& child : children){
				child->update(deltaSeconds);
			}
		}

		void SceneNode::addChild(std::shared_ptr<SceneNode> child){
			children.push_back(child);
			child->parent = weak_from_this();
		}

		SceneNode& SceneNode::operator=(const SceneNode& other) = default;

		SceneNode& SceneNode::operator=(SceneNode&& other) noexcept = default;

		math::Matrix4f SceneNode::getWorldTransform(){
			const auto&[rotVector, rotAngle] = localRotation;

			math::Matrix4f translation	= math::translate(math::Matrix4f(1.0f), localPosition);
			math::Matrix4f rotation		= math::rotate(math::Matrix4f(1.0f), rotAngle, rotVector);
			math::Matrix4f scale		= math::scale(math::Matrix4f(1.0f), localScale);

			math::Matrix4f transform = translation * rotation * scale;

			if(std::shared_ptr<SceneNode> parent = this->parent.lock()){
				return parent->getWorldTransform() * transform;
			} else{
				return transform;
			}
		}
	}
}