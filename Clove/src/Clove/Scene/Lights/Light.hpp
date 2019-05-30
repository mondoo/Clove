#pragma once

#include "../SceneNode.hpp"

namespace clv::scene{
	class Light : public SceneNode{
		//VARIABLES
	private:
		math::Vector3f ambientColour = math::Vector3f(0.01f, 0.01f, 0.01f);
		math::Vector3f diffuseColour = math::Vector3f(0.75f, 0.75f, 0.75f);
		math::Vector3f specularColour = math::Vector3f(1.0f, 1.0f, 1.0f);

		//FUNCTIONS
	public:
		Light();
		Light(const Light& other);
		Light(Light&& other) noexcept;
		Light& operator=(const Light& other);
		Light& operator=(Light&&) noexcept;
		virtual ~Light();

		inline void setAmbientColour(const math::Vector3f& colour);
		inline const math::Vector3f& getAmbientColour() const;

		inline void setDiffuseColour(const math::Vector3f& colour);
		inline const math::Vector3f& getDiffuseColour() const;

		inline void setSpecularColour(const math::Vector3f& colour);
		inline const math::Vector3f& getSpecularColour() const;
	};
}

#include "Light.inl"