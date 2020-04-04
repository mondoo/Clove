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
				farDist
			},
			{},
		};
	}

	PointLightComponent::PointLightComponent(const PointLightComponent& other) = default;

	PointLightComponent::PointLightComponent(PointLightComponent&& other) noexcept = default;

	PointLightComponent& PointLightComponent::operator=(const PointLightComponent& other) = default;

	PointLightComponent& PointLightComponent::operator=(PointLightComponent&&) noexcept = default;

	PointLightComponent::~PointLightComponent() = default;

	void PointLightComponent::setAmbientColour(const mth::vec3f& colour){
		lightData.data.ambient = colour;
	}

	const mth::vec3f& PointLightComponent::getAmbientColour() const{
		return lightData.data.ambient;
	}

	void PointLightComponent::setDiffuseColour(const mth::vec3f& colour){
		lightData.data.diffuse = colour;
	}

	const mth::vec3f& PointLightComponent::getDiffuseColour() const{
		return lightData.data.diffuse;
	}

	void PointLightComponent::setSpecularColour(const mth::vec3f& colour){
		lightData.data.specular = colour;
	}

	const mth::vec3f& PointLightComponent::getSpecularColour() const{
		return lightData.data.specular;
	}

	float PointLightComponent::getConstant() const{
		return lightData.data.constant;
	}

	void PointLightComponent::setLinear(float linear){
		lightData.data.linear = linear;
	}

	float PointLightComponent::getLinear() const{
		return lightData.data.linear;
	}

	void PointLightComponent::setQuadratic(float quadratic){
		lightData.data.quadratic = quadratic;
	}

	float PointLightComponent::getQuadratic() const{
		return lightData.data.quadratic;
	}
}