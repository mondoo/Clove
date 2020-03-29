#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

namespace tnc::ecs::_3D {
	class DirectionalLightComponent : public Component<DirectionalLightComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		rnd::DirectionalLight lightData;

		static constexpr float size = 10.0f;
		static constexpr float nearDist = 0.5f;
		static constexpr float farDist = 100.0f;
		clv::mth::mat4f shadowProj = clv::mth::createOrthographicMatrix(-size, size, -size, size, nearDist, farDist);

		//FUNCTIONS
	public:
		DirectionalLightComponent();

		DirectionalLightComponent(const DirectionalLightComponent& other);
		DirectionalLightComponent(DirectionalLightComponent&& other) noexcept;

		DirectionalLightComponent& operator=(const DirectionalLightComponent& other);
		DirectionalLightComponent& operator=(DirectionalLightComponent&&) noexcept;

		virtual ~DirectionalLightComponent();

		void setDirection(const clv::mth::vec3f& direction);
		const clv::mth::vec3f& getDirection() const;
	};
}