#include "Tunic/ECS/3D/Components/PointLightComponent.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	PointLightComponent::PointLightComponent(){
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

	PointLightComponent::PointLightComponent(const PointLightComponent& other) = default;

	PointLightComponent::PointLightComponent(PointLightComponent&& other) noexcept = default;

	PointLightComponent& PointLightComponent::operator=(const PointLightComponent& other) = default;

	PointLightComponent& PointLightComponent::operator=(PointLightComponent&&) noexcept = default;

	PointLightComponent::~PointLightComponent() = default;

	void PointLightComponent::setAmbientColour(const mth::vec3f& colour){
		lightData.intensity.ambient = colour;
	}

	const mth::vec3f& PointLightComponent::getAmbientColour() const{
		return lightData.intensity.ambient;
	}

	void PointLightComponent::setDiffuseColour(const mth::vec3f& colour){
		lightData.intensity.diffuse = colour;
	}

	const mth::vec3f& PointLightComponent::getDiffuseColour() const{
		return lightData.intensity.diffuse;
	}

	void PointLightComponent::setSpecularColour(const mth::vec3f& colour){
		lightData.intensity.specular = colour;
	}

	const mth::vec3f& PointLightComponent::getSpecularColour() const{
		return lightData.intensity.specular;
	}

	float PointLightComponent::getConstant() const{
		return lightData.intensity.constant;
	}

	void PointLightComponent::setLinear(float linear){
		lightData.intensity.linear = linear;
	}

	float PointLightComponent::getLinear() const{
		return lightData.intensity.linear;
	}

	void PointLightComponent::setQuadratic(float quadratic){
		lightData.intensity.quadratic = quadratic;
	}

	float PointLightComponent::getQuadratic() const{
		return lightData.intensity.quadratic;
	}
}