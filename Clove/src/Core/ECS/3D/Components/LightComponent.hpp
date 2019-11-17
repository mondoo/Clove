#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Graphics/ShaderBufferTypes.hpp"

namespace clv::ecs::_3D{
	class LightComponent : public Component<LightComponent>{
		friend class LightSystem;

		//VARIABLES
	private:
		math::Vector3f ambientColour = math::Vector3f(0.01f, 0.01f, 0.01f);
		math::Vector3f diffuseColour = math::Vector3f(0.75f, 0.75f, 0.75f);
		math::Vector3f specularColour = math::Vector3f(1.0f, 1.0f, 1.0f);

		//NOTE: treating every light as a point light for now
		float constant = 1.0f;
		float linear = 0.0014f;
		float quadratic = 0.000007f;

		gfx::PointLightData lightData;

		float aspect = 1.0f; //shadow width / shadow height
		float nearDist = 0.5f;
		float farDist = 25.0f;
		math::Matrix4f shadowProj = math::createPerspectiveMatrix(math::asRadians(90.0f), aspect, nearDist, farDist);

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