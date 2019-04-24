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
			CLV_API inline void setDirection(const math::Vector3f& direction);
			CLV_API inline const math::Vector3f& getDirection() const;
		};
	}
}

#include "DirectionalLightSceneNode.inl"