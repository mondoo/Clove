#pragma once

#include "Clove/Core/Graphics/GraphicsConstants.hpp"

namespace clv::gfx{
	struct MaterialData {
		alignas(16) float sininess;
	};

	struct ViewData {
		mth::mat4f view;
		mth::mat4f projection;
	};

	struct ViewPos {
		alignas(16) mth::vec3f pos;
	};

	struct CameraRenderData {
		mth::vec3f position;
		mth::mat4f lookAt;
		mth::mat4f projection;
	};

	struct VertexData {
		mth::mat4f model;
		mth::mat4f normalMatrix;
	};

	struct PointLightIntesity {
		mth::vec3f position = {};
		float constant = 0;
		mth::vec3f ambient = {};
		float linear = 0;
		mth::vec3f diffuse = {};
		float quadratic = 0;
		alignas(16) mth::vec3f specular = {};
	};
	struct PointLightData {
		PointLightIntesity intensity = {};
		std::array<mth::mat4f, 6> shadowTransforms = {};
		float farPlane;
	};
	struct PointLightShaderData {
		std::array<PointLightIntesity, MAX_LIGHTS> intensities = {};
	};
	struct PointShadowShaderData {
		std::array<mth::mat4f, 6> shadowTransforms = {};
	};
	struct PointShadowData {
		mth::vec3f lightPos = {};
		float farPlane = 0;
	};
	struct PointShadowDepthData { //It's not depth - this needs a better name (so does the BBP for it)
		std::array<PointShadowData, MAX_LIGHTS> depths = {};
	};

	struct LightNumAlignment {
		alignas(16) uint32 numLights;
	};
}