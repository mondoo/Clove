#include "clvpch.hpp"
#include "LightSceneNode.hpp"

namespace clv{
	namespace scene{
		LightSceneNode::LightSceneNode() = default;

		LightSceneNode::LightSceneNode(const LightSceneNode& other) = default;

		LightSceneNode::LightSceneNode(LightSceneNode&& other) = default;

		LightSceneNode::~LightSceneNode() = default;

		LightSceneNode& LightSceneNode::operator=(const LightSceneNode& other) = default;

		LightSceneNode& LightSceneNode::operator=(LightSceneNode&&) noexcept = default;
	}
}