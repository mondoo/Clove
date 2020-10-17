#pragma once

#include "Bulb/Rendering/RenderingConstants.hpp"

namespace blb::rnd {
	struct alignas(16) MaterialData {
		float sininess;
	};

	struct ViewData {
		clv::mth::mat4f view;
		clv::mth::mat4f projection;
	};

	struct alignas(16) ViewPos {
		clv::mth::vec3f pos;
	};

    //Lighting data passed to GPU
	struct DirectionalLightData {
		alignas(16) clv::mth::vec3f direction{};

		alignas(16) clv::mth::vec3f ambient{};
		alignas(16) clv::mth::vec3f diffuse{};
		alignas(16) clv::mth::vec3f specular{};
	};
	struct PointLightData {
		clv::mth::vec3f position{};

		float constant = 0;
		clv::mth::vec3f ambient{};
		float linear = 0;
		clv::mth::vec3f diffuse{};
		float quadratic = 0;
		clv::mth::vec3f specular{};

		float farPlane = 0;
	};
	struct LightDataArray {
		std::array<DirectionalLightData, MAX_LIGHTS> directionalLights{};
		std::array<PointLightData, MAX_LIGHTS> pointLights{};
	};

	struct alignas(16) LightCount {
		int32_t numDirectional{ 0 };
		int32_t numPoint{ 0 };
	};

	//Lighting data needed shadows
	struct DirectionalShadowTransform {
		clv::mth::mat4f shadowTransform;
	};
	struct DirectionalShadowTransformArray {
		std::array<clv::mth::mat4f, MAX_LIGHTS> transforms;
		clv::mth::mat4f& operator[](const size_t index) {
			return transforms[index];
		}
	};
	struct PointShadowTransform {
		std::array<clv::mth::mat4f, 6> shadowTransforms{};
	};

	//Lighting data passed to renderer
	struct DirectionalLight {
		DirectionalLightData data{};
		clv::mth::mat4f shadowTransform{};
	};
	struct PointLight {
		PointLightData data{};
		std::array<clv::mth::mat4f, 6> shadowTransforms{};
	};

	struct alignas(16) NumberAlignment {
		int32_t num;
	};

	struct SkeletalData {
        std::array<clv::mth::mat4f, MAX_JOINTS> jointTransforms;
    };
}