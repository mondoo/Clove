#include "Clove/Core/ECS/3D/Components/LightComponent.hpp"

namespace clv::ecs::_3D{
	LightComponent::LightComponent(){
		lightData = {
			{ { 0.0f, 0.0f, 0.0f },
			constant,
			getAmbientColour(),
			linear,
			getDiffuseColour(),
			quadratic,
			getSpecularColour(), },
			{},
			farDist
		};
	}

	LightComponent::LightComponent(const LightComponent& other) = default;

	LightComponent::LightComponent(LightComponent&& other) noexcept = default;

	LightComponent& LightComponent::operator=(const LightComponent& other) = default;

	LightComponent& LightComponent::operator=(LightComponent&&) noexcept = default;

	LightComponent::~LightComponent() = default;

	void LightComponent::setAmbientColour(const mth::vec3f& colour){
		ambientColour = colour;
	}

	const mth::vec3f& LightComponent::getAmbientColour() const{
		return ambientColour;
	}

	void LightComponent::setDiffuseColour(const mth::vec3f& colour){
		diffuseColour = colour;
	}

	const mth::vec3f& LightComponent::getDiffuseColour() const{
		return diffuseColour;
	}

	void LightComponent::setSpecularColour(const mth::vec3f& colour){
		specularColour = colour;
	}

	const mth::vec3f& LightComponent::getSpecularColour() const{
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