#include "clvpch.hpp"
#include "LightComponent.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Application.hpp"

namespace clv::ecs{
	LightComponent::LightComponent(){
		lightData = {
			{0.0f, 0.0f, 0.0f},
			getAmbientColour(),
			getDiffuseColour(),
			getSpecularColour(),
			constant,
			linear,
			quadratic
		};

		sbo = gfx::BindableFactory::createShaderBufferObject<LightData>(gfx::ShaderType::Pixel, gfx::BBP_PointLightData);
		sbo->bind();
	}

	LightComponent::LightComponent(LightComponent&& other) noexcept = default;

	LightComponent& LightComponent::operator=(LightComponent&&) noexcept = default;

	LightComponent::~LightComponent() = default;

	void LightComponent::setAmbientColour(const math::Vector3f& colour){
		ambientColour = colour;
	}

	const math::Vector3f& LightComponent::getAmbientColour() const{
		return ambientColour;
	}

	void LightComponent::setDiffuseColour(const math::Vector3f& colour){
		diffuseColour = colour;
	}

	const math::Vector3f& LightComponent::getDiffuseColour() const{
		return diffuseColour;
	}

	void LightComponent::setSpecularColour(const math::Vector3f& colour){
		specularColour = colour;
	}

	const math::Vector3f& LightComponent::getSpecularColour() const{
		return specularColour;
	}

	float LightComponent::getConstant() const{
		return constant;
	}

	void LightComponent::setLinear(float linear){
		this->linear = linear;
	}

	float LightComponent::getLinear() const{
		return linear;
	}

	void LightComponent::setQuadratic(float quadratic){
		this->quadratic = quadratic;
	}

	float LightComponent::getQuadratic() const{
		return quadratic;
	}
}