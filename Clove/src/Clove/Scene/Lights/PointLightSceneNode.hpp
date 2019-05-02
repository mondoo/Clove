#pragma once

#include "LightSceneNode.hpp"

namespace clv{
	namespace scene{
		class PointLightSceneNode : public LightSceneNode{
			//VARIABLES
		private:
			float constant = 1.0f;
			float linear = 0.0014f;
			float quadratic = 0.000007f;

			//FUNCTIONS
		public:
			CLV_API PointLightSceneNode();
			CLV_API PointLightSceneNode(const PointLightSceneNode& other);
			CLV_API PointLightSceneNode(PointLightSceneNode&& other) noexcept;

			CLV_API virtual ~PointLightSceneNode();

			CLV_API inline float getConstant() const;

			CLV_API inline void setLinear(float linear);
			CLV_API inline float getLinear() const;

			CLV_API inline void setQuadratic(float quadratic);
			CLV_API inline float getQuadratic() const;

			CLV_API PointLightSceneNode& operator=(const PointLightSceneNode& other);
			CLV_API PointLightSceneNode& operator=(PointLightSceneNode&& other) noexcept;
		};
	}
}

#include "PointLightSceneNode.inl"