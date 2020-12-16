#pragma once

#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/ECS/Component.hpp>
#include <Clove/Maths/Matrix.hpp>
#include <Clove/Maths/Vector.hpp>
#include <Clove/Maths/MathsHelpers.hpp>

namespace garlic::clove {
    class DirectionalLightComponent : public Component<DirectionalLightComponent> {
        friend class RenderLayer;

        //VARIABLES
    private:
        DirectionalLight lightData;

        static float constexpr size{ 50.0f };
        static float constexpr nearDist{ 0.5f };
        static float constexpr farDist{ 1000.0f };
        mat4f shadowProj{ createOrthographicMatrix(-size, size, -size, size, nearDist, farDist) };

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
        inline void setDirection(vec3f direction);
        inline vec3f const &getDirection() const;
    };
}

#include "DirectionalLightComponent.inl"