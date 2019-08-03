#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs::d3{
	class LightComponent : public Component{
		friend class LightSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x62fe5696; //VS generated GUID

	private:
		math::Vector3f ambientColour = math::Vector3f(0.01f, 0.01f, 0.01f);
		math::Vector3f diffuseColour = math::Vector3f(0.75f, 0.75f, 0.75f);
		math::Vector3f specularColour = math::Vector3f(1.0f, 1.0f, 1.0f);

		//NOTE: treating every light as a point light for now
		float constant = 1.0f;
		float linear = 0.0014f;
		float quadratic = 0.000007f;

		gfx::PointLightData lightData;

		//FUNCTIONS
	public:
		LightComponent();
		LightComponent(const LightComponent& other) = delete;
		LightComponent(LightComponent&& other) noexcept;
		LightComponent& operator=(const LightComponent& other) = delete;
		LightComponent& operator=(LightComponent&&) noexcept;
		virtual ~LightComponent();

		void setAmbientColour(const math::Vector3f& colour);
		const math::Vector3f& getAmbientColour() const;

		void setDiffuseColour(const math::Vector3f& colour);
		const math::Vector3f& getDiffuseColour() const;

		void setSpecularColour(const math::Vector3f& colour);
		const math::Vector3f& getSpecularColour() const;

		//NOTE: treating every light as a point light for now
		float getConstant() const;

		void setLinear(float linear);
		float getLinear() const;

		void setQuadratic(float quadratic);
		float getQuadratic() const;
	};
}