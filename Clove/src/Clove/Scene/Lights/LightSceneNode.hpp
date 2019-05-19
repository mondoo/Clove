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
			LightSceneNode();
			LightSceneNode(const LightSceneNode& other);
			LightSceneNode(LightSceneNode&& other);

			virtual ~LightSceneNode();

			inline void setAmbientColour(const math::Vector3f& colour);
			inline const math::Vector3f& getAmbientColour() const;

			inline void setDiffuseColour(const math::Vector3f& colour);
			inline const math::Vector3f& getDiffuseColour() const;

			inline void setSpecularColour(const math::Vector3f& colour);
			inline const math::Vector3f& getSpecularColour() const;

			LightSceneNode& operator=(const LightSceneNode& other);
			LightSceneNode& operator=(LightSceneNode&&) noexcept;
		};
	}
}

#include "LightSceneNode.inl"