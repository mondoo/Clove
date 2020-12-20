#pragma once

#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Maths/Matrix.hpp>
#include <Clove/Maths/Vector.hpp>
#include <Clove/Maths/MathsHelpers.hpp>

namespace garlic::clove {
    class PointLightComponent {
        friend class RenderLayer;

        //VARIABLES
    private:
        PointLight lightData;

        static float constexpr aspect{ 1.0f };//shadow width / shadow height
        static float constexpr nearDist{ 0.5f };
        static float constexpr farDist{ 100.0f };
        mat4f shadowProj{ createPerspectiveMatrix(asRadians(90.0f), aspect, nearDist, farDist) };

        //FUNCTIONS
    public:
        PointLightComponent();

        PointLightComponent(PointLightComponent const &other);
        PointLightComponent(PointLightComponent &&other) noexcept;

        PointLightComponent &operator=(PointLightComponent const &other);
        PointLightComponent &operator=(PointLightComponent &&) noexcept;

        ~PointLightComponent();

        inline void setAmbientColour(vec3f colour);
        inline vec3f const &getAmbientColour() const;

        inline void setDiffuseColour(vec3f colour);
        inline vec3f const &getDiffuseColour() const;

        inline void setSpecularColour(vec3f colour);
        inline vec3f const &getSpecularColour() const;

        inline float getConstant() const;

        inline void setLinear(float linear);
        inline float getLinear() const;

        inline void setQuadratic(float quadratic);
        inline float getQuadratic() const;
    };
}

#include "PointLightComponent.inl"