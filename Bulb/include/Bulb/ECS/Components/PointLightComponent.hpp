#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

namespace blb::ecs {
	class PointLightComponent : public Component<PointLightComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		blb::rnd::PointLight lightData;

		static constexpr float aspect = 1.0f; //shadow width / shadow height
		static constexpr float nearDist = 0.5f;
		static constexpr float farDist = 100.0f;
		clv::mth::mat4f shadowProj = clv::mth::createPerspectiveMatrix(clv::mth::asRadians(90.0f), aspect, nearDist, farDist);

		//FUNCTIONS
	public:
		PointLightComponent();

		PointLightComponent(const PointLightComponent& other);
		PointLightComponent(PointLightComponent&& other) noexcept;

		PointLightComponent& operator=(const PointLightComponent& other);
		PointLightComponent& operator=(PointLightComponent&&) noexcept;

		~PointLightComponent();

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