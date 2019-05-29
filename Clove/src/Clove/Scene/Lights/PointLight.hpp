#pragma once

#include "Light.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::scene{
	struct LightData{
		alignas(16) math::Vector3f position;

		alignas(16) math::Vector3f ambient;
		alignas(16) math::Vector3f diffuse;
		alignas(16) math::Vector3f specular;

		/*alignas(16)*/ float constant;
		/*alignas(16)*/ float linear;
		/*alignas(16)*/ float quadratic;
	};

	class PointLight : public Light{
		//VARIABLES
	private:
		float constant = 1.0f;
		float linear = 0.0014f;
		float quadratic = 0.000007f;

		LightData lightData;

		std::unique_ptr<gfx::ShaderBufferObject<LightData>> sbo;

		//FUNCTIONS
	public:
		PointLight();
		PointLight(const PointLight& other) = delete; //TODO: has to be this way because of the sbo (should make custom ones for it)
		PointLight(PointLight&& other) noexcept;
		PointLight& operator=(const PointLight& other) = delete; //TODO: has to be this way because of the sbo (should make custom ones for it)
		PointLight& operator=(PointLight&& other) noexcept;
		virtual ~PointLight();

		virtual void update(float deltaSeconds) override;

		inline float getConstant() const;

		inline void setLinear(float linear);
		inline float getLinear() const;

		inline void setQuadratic(float quadratic);
		inline float getQuadratic() const;
	};
}

#include "PointLight.inl"