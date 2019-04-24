#pragma once

#include "LightSceneNode.hpp"

namespace clv{
	namespace scene{
		class PointLightSceneNode : public LightSceneNode{
			//VARIABLES
		private:
			const float constant = 1.0f;
			float linear = 0.0014f;
			float quadratic = 0.000007f;

			//FUNCTIONS
		public:
			CLV_API inline float getConstant() const;

			CLV_API inline void setLinear(float linear);
			CLV_API inline float getLinear() const;

			CLV_API inline void setQuadratic(float quadratic);
			CLV_API inline float getQuadratic() const;
		};
	}
}

#include "PointLightSceneNode.inl"