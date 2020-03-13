#pragma once

#include "Tunic/Rendering/RenderingConstants.hpp"

namespace tnc::rnd{
	struct MaterialData {
		alignas(16) float sininess;
	};

	struct ViewData {
		clv::mth::mat4f view;
		clv::mth::mat4f projection;
	};

	struct ViewPos {
		alignas(16) clv::mth::vec3f pos;
	};

	struct CameraRenderData {
		clv::mth::vec3f position;
		clv::mth::mat4f lookAt;
		clv::mth::mat4f projection;
	};

	struct VertexData {
		clv::mth::mat4f model;
		clv::mth::mat4f normalMatrix;
	};

	//Lighting data needed for point lighting
	struct PointLightIntesity {
		clv::mth::vec3f position{};
		float constant = 0;
		clv::mth::vec3f ambient{};
		float linear = 0;
		clv::mth::vec3f diffuse{};
		float quadratic = 0;
		clv::mth::vec3f specular{};
		float farPlane = 0;
	};
	struct PointLightIntensityArray {
		std::array<PointLightIntesity, MAX_LIGHTS> intensities{};

		PointLightIntesity& operator[](size_t index) {
			return intensities[index];
		}
	};

	//Lighting data needed for directional lighting
	struct DirLightIntensity {
		clv::mth::vec3f direction{};
		float constant = 0;
		clv::mth::vec3f ambient{};
		float linear = 0;
		clv::mth::vec3f diffuse{};
		float quadratic = 0;
		alignas(16) clv::mth::vec3f specular{};
	};

	//Lighting data needed for point shadows
	struct PointShadowTransform {
		std::array<clv::mth::mat4f, 6> shadowTransforms{};
	};

	//Lighting data passed to renderer
	struct PointLightData {
		PointLightIntesity intensity{};
		std::array<clv::mth::mat4f, 6> shadowTransforms{};
	};

	struct DirLightData {
		DirLightIntensity intensity{};
		//std::array<clv::mth::mat4f, 6> shadowTransforms{};
		//float farPlane;
	};
	
	struct NumberAlignment {
		alignas(16) uint32_t numLights;
	};
}