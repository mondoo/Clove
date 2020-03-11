#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

namespace tnc::ecs::_3D {
	class DirectionalLightComponent : public Component<DirectionalLightComponent> {
		//VARIABLES
	private:
		rnd::DirLightData lightData;

		//FUNCTIONS
	public:
		DirectionalLightComponent();

		DirectionalLightComponent(const DirectionalLightComponent& other);
		DirectionalLightComponent(DirectionalLightComponent&& other) noexcept;

		DirectionalLightComponent& operator=(const DirectionalLightComponent& other);
		DirectionalLightComponent& operator=(DirectionalLightComponent&&) noexcept;

		virtual ~DirectionalLightComponent();
	};
}