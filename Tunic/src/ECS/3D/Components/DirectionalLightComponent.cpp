#include "Tunic/ECS/3D/Components/DirectionalLightComponent.hpp"

namespace tnc::ecs::_3D {
	DirectionalLightComponent::DirectionalLightComponent() {
		lightData = {
			{
				{ 0.0f, 0.0f, 0.0f }, //Direction
				clv::mth::vec3f(0.01f, 0.01f, 0.01f), //Ambient colour
				clv::mth::vec3f(0.75f, 0.75f, 0.75f),//Diffuse colour
				clv::mth::vec3f(1.0f, 1.0f, 1.0f),//Specular colour
			},
			//{},
			//farDist
		};
	}

	DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponent& other) = default;

	DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent&& other) noexcept = default;

	DirectionalLightComponent& DirectionalLightComponent::operator=(const DirectionalLightComponent& other) = default;

	DirectionalLightComponent& DirectionalLightComponent::operator=(DirectionalLightComponent&&) noexcept = default;

	DirectionalLightComponent::~DirectionalLightComponent() = default;
}