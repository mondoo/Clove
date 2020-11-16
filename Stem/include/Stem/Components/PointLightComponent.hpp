#pragma once

#include "Stem/Rendering/ShaderBufferTypes.hpp"

#include <Bulb/ECS/Component.hpp>

namespace garlic::inline stem {
    class PointLightComponent : public blb::ecs::Component<PointLightComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        PointLight lightData;

        static float constexpr aspect{ 1.0f };//shadow width / shadow height
        static float constexpr nearDist{ 0.5f };
        static float constexpr farDist{ 100.0f };
        clv::mth::mat4f shadowProj{ clv::mth::createPerspectiveMatrix(clv::mth::asRadians(90.0f), aspect, nearDist, farDist) };

        //FUNCTIONS
    public:
        PointLightComponent();

        PointLightComponent(PointLightComponent const &other);
        PointLightComponent(PointLightComponent &&other) noexcept;

        PointLightComponent &operator=(PointLightComponent const &other);
        PointLightComponent &operator=(PointLightComponent &&) noexcept;

        ~PointLightComponent();

        inline void setAmbientColour(clv::mth::vec3f colour);
        inline clv::mth::vec3f const &getAmbientColour() const;

        inline void setDiffuseColour(clv::mth::vec3f colour);
        inline clv::mth::vec3f const &getDiffuseColour() const;

        inline void setSpecularColour(clv::mth::vec3f colour);
        inline clv::mth::vec3f const &getSpecularColour() const;

        inline float getConstant() const;

        inline void setLinear(float linear);
        inline float getLinear() const;

        inline void setQuadratic(float quadratic);
        inline float getQuadratic() const;
    };
}

#include "PointLightComponent.inl"