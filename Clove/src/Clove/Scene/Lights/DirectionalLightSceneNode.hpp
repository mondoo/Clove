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
			DirectionalLightSceneNode();
			DirectionalLightSceneNode(const DirectionalLightSceneNode& other);
			DirectionalLightSceneNode(DirectionalLightSceneNode&& other) noexcept;

			virtual ~DirectionalLightSceneNode();

			inline void setDirection(const math::Vector3f& direction);
			inline const math::Vector3f& getDirection() const;

			DirectionalLightSceneNode& operator=(const DirectionalLightSceneNode& other);
			DirectionalLightSceneNode& operator=(DirectionalLightSceneNode&& other) noexcept;
		};
	}
}

#include "DirectionalLightSceneNode.inl"