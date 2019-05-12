#pragma once

#include "../SceneNode.hpp"

namespace clv{
	namespace scene{
		class LightSceneNode : public SceneNode{
			//VARIABLES
		private:
			math::Vector3f ambientColour = math::Vector3f(0.5f, 0.5f, 0.5f);
			math::Vector3f diffuseColour = math::Vector3f(0.75f, 0.75f, 0.75f);
			math::Vector3f specularColour = math::Vector3f(1.0f, 1.0f, 1.0f);

			//FUNCTIONS
		public:
			CLV_API LightSceneNode();
			CLV_API LightSceneNode(const LightSceneNode& other);
			CLV_API LightSceneNode(LightSceneNode&& other);

			CLV_API virtual ~LightSceneNode();

			CLV_API inline void setAmbientColour(const math::Vector3f& colour);
			CLV_API inline const math::Vector3f& getAmbientColour() const;

			CLV_API inline void setDiffuseColour(const math::Vector3f& colour);
			CLV_API inline const math::Vector3f& getDiffuseColour() const;

			CLV_API inline void setSpecularColour(const math::Vector3f& colour);
			CLV_API inline const math::Vector3f& getSpecularColour() const;

			CLV_API LightSceneNode& operator=(const LightSceneNode& other);
			CLV_API LightSceneNode& operator=(LightSceneNode&&) noexcept;
		};
	}
}

#include "LightSceneNode.inl"