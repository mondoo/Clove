#include "clvpch.hpp"
#include "PointLight.hpp"

#include "Clove/Graphics/Bindables/BindableFactory.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::scene{
	PointLight::PointLight(){
		lightData = {
			getPosition(),
			getAmbientColour(),
			getDiffuseColour(),
			getSpecularColour(),
			constant,
			linear,
			quadratic
		};

		sbo = gfx::BindableFactory::createShaderBufferObject<LightData>(gfx::ShaderTypes::Pixel, gfx::BBP_PointLightData);
		sbo->bind(Application::get().getRenderer());
	}

	//PointLight::PointLight(const PointLight& other) = default;

	PointLight::PointLight(PointLight&& other) noexcept = default;

	//PointLight& PointLight::operator=(const PointLight& other) = default;

	PointLight& PointLight::operator=(PointLight&& other) noexcept = default;

	PointLight::~PointLight() = default;

	void PointLight::update(float deltaSeconds){
		Light::update(deltaSeconds);

		lightData.position = getPosition();

		sbo->update(lightData, Application::get().getRenderer());
	}
}