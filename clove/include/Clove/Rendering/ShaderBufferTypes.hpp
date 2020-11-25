#pragma once

#include "Clove/Rendering/RenderingConstants.hpp"

#include <Clove/Maths/Matrix.hpp>
#include <Clove/Maths/Vector.hpp>
#include <array>

namespace garlic::clove {
    struct alignas(16) MaterialData {
        float sininess;
    };

    struct ModelData {
        mat4f model;
        mat4f inverseTransposeModel;
    };

    struct ViewData {
        mat4f view;
        mat4f projection;
    };

    //Lighting data passed to GPU
    struct DirectionalLightData {
        alignas(16) vec3f direction{};

        alignas(16) vec3f ambient{};
        alignas(16) vec3f diffuse{};
        alignas(16) vec3f specular{};
    };
    struct PointLightData {
        vec3f position{};

        float constant = 0;
        vec3f ambient{};
        float linear = 0;
        vec3f diffuse{};
        float quadratic = 0;
        vec3f specular{};

        float farPlane = 0;
    };
    struct LightDataArray {
        std::array<DirectionalLightData, MAX_LIGHTS> directionalLights{};
        std::array<PointLightData, MAX_LIGHTS> pointLights{};
    };

    struct LightCount {
        int32_t numDirectional{ 0 };
        int32_t numPoint{ 0 };
    };

    //Lighting data needed shadows
    struct DirectionalShadowTransformArray {
        std::array<mat4f, MAX_LIGHTS> transforms;
        mat4f &operator[](const size_t index) {
            return transforms[index];
        }
    };

    //For renderer - TODO Move into that header
    struct DirectionalLight {
        DirectionalLightData data{};
        mat4f shadowTransform{};
    };
    struct PointLight {
        PointLightData data{};
        std::array<mat4f, 6> shadowTransforms{};
    };

    struct SkeletalData {
        std::array<mat4f, MAX_JOINTS> jointTransforms;
    };
}