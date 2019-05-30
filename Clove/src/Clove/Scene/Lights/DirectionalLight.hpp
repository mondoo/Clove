#pragma once

#include "Light.hpp"

namespace clv{
	namespace scene{
		class DirectionalLight : public Light{
			//VARIABLES
		private:
			math::Vector3f direction = math::Vector3f(-0.2f, -1.0f, -0.3f);

			//FUNCTIONS
		public:
			DirectionalLight();
			DirectionalLight(const DirectionalLight& other);
			DirectionalLight(DirectionalLight&& other) noexcept;

			virtual ~DirectionalLight();

			inline void setDirection(const math::Vector3f& direction);
			inline const math::Vector3f& getDirection() const;

			DirectionalLight& operator=(const DirectionalLight& other);
			DirectionalLight& operator=(DirectionalLight&& other) noexcept;
		};
	}
}

#include "DirectionalLight.inl"