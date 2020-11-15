#pragma once

#include <Bulb/ECS/Component.hpp>
#include <Bulb/Rendering/ShaderBufferTypes.hpp>

namespace garlic::inline stem {
    class DirectionalLightComponent : public blb::ecs::Component<DirectionalLightComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        blb::rnd::DirectionalLight lightData;

        static float constexpr size{ 50.0f };
        static float constexpr nearDist{ 0.5f };
        static float constexpr farDist{ 1000.0f };
        clv::mth::mat4f shadowProj{ clv::mth::createOrthographicMatrix(-size, size, -size, size, nearDist, farDist) };

        //FUNCTIONS
    public:
        DirectionalLightComponent();

        DirectionalLightComponent(DirectionalLightComponent const &other);
        DirectionalLightComponent(DirectionalLightComponent &&other) noexcept;

        DirectionalLightComponent &operator=(DirectionalLightComponent const &other);
        DirectionalLightComponent &operator=(DirectionalLightComponent &&) noexcept;

        ~DirectionalLightComponent();

        /**
		 * @brief Sets the direction the light faces.
		 */
        inline void setDirection(clv::mth::vec3f direction);
        inline clv::mth::vec3f const &getDirection() const;
    };
}

#include "DirectionalLightComponent.inl"