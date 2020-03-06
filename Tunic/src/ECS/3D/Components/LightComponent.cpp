#include "Tunic/ECS/3D/Components/LightComponent.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	LightComponent::LightComponent(){
		lightData = {
			{	{ 0.0f, 0.0f, 0.0f },
				1.0f, //Constant
				mth::vec3f(0.01f, 0.01f, 0.01f), //Ambient colour
				0.0014f, //Linear
				clv::mth::vec3f(0.75f, 0.75f, 0.75f), //Diffuse colour
				0.000007f, //Quadratic
				clv::mth::vec3f(1.0f, 1.0f, 1.0f), //Specular colour
			},
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
		lightData.intensity.ambient = colour;
	}

	const mth::vec3f& LightComponent::getAmbientColour() const{
		return lightData.intensity.ambient;
	}

	void LightComponent::setDiffuseColour(const mth::vec3f& colour){
		lightData.intensity.diffuse = colour;
	}

	const mth::vec3f& LightComponent::getDiffuseColour() const{
		return lightData.intensity.diffuse;
	}

	void LightComponent::setSpecularColour(const mth::vec3f& colour){
		lightData.intensity.specular = colour;
	}

	const mth::vec3f& LightComponent::getSpecularColour() const{
		return lightData.intensity.specular;
	}

	float LightComponent::getConstant() const{
		return lightData.intensity.constant;
	}

	void LightComponent::setLinear(float linear){
		lightData.intensity.linear = linear;
	}

	float LightComponent::getLinear() const{
		return lightData.intensity.linear;
	}

	void LightComponent::setQuadratic(float quadratic){
		lightData.intensity.quadratic = quadratic;
	}

	float LightComponent::getQuadratic() const{
		return lightData.intensity.quadratic;
	}
}