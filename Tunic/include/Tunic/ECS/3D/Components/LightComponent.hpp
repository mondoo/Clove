#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

namespace tnc::ecs::_3D{
	class LightComponent : public Component<LightComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		tnc::rnd::PointLightData lightData;

		float aspect = 1.0f; //shadow width / shadow height
		float nearDist = 0.5f;
		float farDist = 100.0f;
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