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

	struct PointLightIntesity {
		clv::mth::vec3f position = {};
		float constant = 0;
		clv::mth::vec3f ambient = {};
		float linear = 0;
		clv::mth::vec3f diffuse = {};
		float quadratic = 0;
		alignas(16) clv::mth::vec3f specular = {};
	};
	struct PointLightData {
		PointLightIntesity intensity = {};
		std::array<clv::mth::mat4f, 6> shadowTransforms = {};
		float farPlane;
	};
	struct PointLightShaderData {
		std::array<PointLightIntesity, MAX_LIGHTS> intensities = {};
	};
	struct PointShadowShaderData {
		std::array<clv::mth::mat4f, 6> shadowTransforms = {};
	};
	struct PointShadowData {
		clv::mth::vec3f lightPos = {};
		float farPlane = 0;
	};
	struct PointShadowDepthData { //It's not depth - this needs a better name (so does the BBP for it)
		std::array<PointShadowData, MAX_LIGHTS> depths = {};
	};

	struct LightNumAlignment {
		alignas(16) uint32_t numLights;
	};
}