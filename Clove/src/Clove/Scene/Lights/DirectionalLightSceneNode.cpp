#include "clvpch.hpp"
#include "DirectionalLightSceneNode.hpp"

namespace clv{
	namespace scene{
		DirectionalLightSceneNode::DirectionalLightSceneNode() = default;

		DirectionalLightSceneNode::DirectionalLightSceneNode(const DirectionalLightSceneNode& other) = default;

		DirectionalLightSceneNode::DirectionalLightSceneNode(DirectionalLightSceneNode&& other) noexcept{
			direction = std::move(other.direction);
			LightSceneNode(std::move(other));
		}

		DirectionalLightSceneNode::~DirectionalLightSceneNode() = default;

		DirectionalLightSceneNode& DirectionalLightSceneNode::operator=(const DirectionalLightSceneNode& other) = default;

		DirectionalLightSceneNode& DirectionalLightSceneNode::operator=(DirectionalLightSceneNode&& other) noexcept = default;
	}
}