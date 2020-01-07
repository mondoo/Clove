#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Clove/Graphics/Core/ShaderBufferTypes.hpp"

namespace tnc::ecs::_3D{
	class LightComponent : public Component<LightComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		clv::mth::vec3f ambientColour	= clv::mth::vec3f(0.01f, 0.01f, 0.01f);
		clv::mth::vec3f diffuseColour	= clv::mth::vec3f(0.75f, 0.75f, 0.75f);
		clv::mth::vec3f specularColour	= clv::mth::vec3f(1.0f, 1.0f, 1.0f);

		//NOTE: treating every light as a point light for now
		float constant = 1.0f;
		float linear = 0.0014f;
		float quadratic = 0.000007f;

		clv::gfx::PointLightData lightData;

		float aspect = 1.0f; //shadow width / shadow height
		float nearDist = 0.5f;
		float farDist = 25.0f;
		clv::mth::mat4f shadowProj = clv::mth::createPerspectiveMatrix(clv::mth::asRadians(90.0f), aspect, nearDist, farDist);

		//FUNCTIONS
	public:
		LightComponent();

		LightComponent(const LightComponent& other);
		LightComponent(LightComponent&& other) noexcept;

		LightComponent& operator=(const LightComponent& other);
		LightComponent& operator=(LightComponent&&) noexcept;

		virtual ~LightComponent();

		void setAmbientColour(const clv::mth::vec3f& colour);
		const clv::mth::vec3f& getAmbientColour() const;

		void setDiffuseColour(const clv::mth::vec3f& colour);
		const clv::mth::vec3f& getDiffuseColour() const;

		void setSpecularColour(const clv::mth::vec3f& colour);
		const clv::mth::vec3f& getSpecularColour() const;

		//NOTE: treating every light as a point light for now
		float getConstant() const;

		void setLinear(float linear);
		float getLinear() const;

		void setQuadratic(float quadratic);
		float getQuadratic() const;
	};
}