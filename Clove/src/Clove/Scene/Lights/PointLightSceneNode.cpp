#include "clvpch.hpp"
#include "PointLightSceneNode.hpp"

namespace clv{
	namespace scene{
		PointLightSceneNode::PointLightSceneNode() = default;

		PointLightSceneNode::PointLightSceneNode(const PointLightSceneNode& other) = default;

		PointLightSceneNode::PointLightSceneNode(PointLightSceneNode&& other) noexcept{
			constant	= other.constant;
			linear		= other.linear;
			quadratic	= other.quadratic;
			LightSceneNode(std::move(other));
		}

		PointLightSceneNode::~PointLightSceneNode() = default;

		PointLightSceneNode& PointLightSceneNode::operator=(const PointLightSceneNode& other) = default;

		PointLightSceneNode& PointLightSceneNode::operator=(PointLightSceneNode&& other) noexcept = default;
	}
}