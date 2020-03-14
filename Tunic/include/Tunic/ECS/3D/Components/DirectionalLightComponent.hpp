#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

namespace tnc::ecs::_3D {
	class DirectionalLightComponent : public Component<DirectionalLightComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		rnd::DirectionalLight lightData;

		//FUNCTIONS
	public:
		DirectionalLightComponent();

		DirectionalLightComponent(const DirectionalLightComponent& other);
		DirectionalLightComponent(DirectionalLightComponent&& other) noexcept;

		DirectionalLightComponent& operator=(const DirectionalLightComponent& other);
		DirectionalLightComponent& operator=(DirectionalLightComponent&&) noexcept;

		virtual ~DirectionalLightComponent();

		void setDirection(const clv::mth::vec3f& direction);
	};
}