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
			PointLightSceneNode();
			PointLightSceneNode(const PointLightSceneNode& other);
			PointLightSceneNode(PointLightSceneNode&& other) noexcept;

			virtual ~PointLightSceneNode();

			inline float getConstant() const;

			inline void setLinear(float linear);
			inline float getLinear() const;

			inline void setQuadratic(float quadratic);
			inline float getQuadratic() const;

			PointLightSceneNode& operator=(const PointLightSceneNode& other);
			PointLightSceneNode& operator=(PointLightSceneNode&& other) noexcept;
		};
	}
}

#include "PointLightSceneNode.inl"