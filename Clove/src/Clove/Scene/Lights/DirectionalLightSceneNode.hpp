#pragma once

#include "LightSceneNode.hpp"

namespace clv{
	namespace scene{
		class DirectionalLightSceneNode : public LightSceneNode{
			//VARIABLES
		private:
			math::Vector3f direction = math::Vector3f(-0.2f, -1.0f, -0.3f);

			//FUNCTIONS
		public:
			CLV_API DirectionalLightSceneNode();
			CLV_API DirectionalLightSceneNode(const DirectionalLightSceneNode& other);
			CLV_API DirectionalLightSceneNode(DirectionalLightSceneNode&& other) noexcept;

			CLV_API virtual ~DirectionalLightSceneNode();

			CLV_API inline void setDirection(const math::Vector3f& direction);
			CLV_API inline const math::Vector3f& getDirection() const;

			CLV_API DirectionalLightSceneNode& operator=(const DirectionalLightSceneNode& other);
			CLV_API DirectionalLightSceneNode& operator=(DirectionalLightSceneNode&& other) noexcept;
		};
	}
}

#include "DirectionalLightSceneNode.inl"