#pragma once

#include "Core/Graphics/GraphicsConstants.hpp"

namespace clv::gfx{
	struct MaterialData {
		alignas(16) float sininess;
	};

	struct ViewData {
		math::Matrix4f view;
		math::Matrix4f projection;
	};

	struct ViewPos {
		alignas(16) math::Vector3f pos;
	};

	struct CameraRenderData {
		math::Vector3f position;
		math::Matrix4f lookAt;
		math::Matrix4f projection;
	};

	struct VertexData {
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct PointLightIntesity {
		math::Vector3f position = {};
		float constant = 0;
		math::Vector3f ambient = {};
		float linear = 0;
		math::Vector3f diffuse = {};
		float quadratic = 0;
		alignas(16) math::Vector3f specular = {};
	};
	struct PointLightData {
		PointLightIntesity intensity = {};
		std::array<math::Matrix4f, 6> shadowTransforms = {};
		float farPlane;
	};
	struct PointLightShaderData {
		std::array<PointLightIntesity, MAX_LIGHTS> intensities = {};
	};
	struct PointShadowShaderData {
		std::array<math::Matrix4f, 6> shadowTransforms = {};
	};
	struct PointShadowData {
		math::Vector3f lightPos = {};
		float farPlane = 0;
	};
	struct PointShadowDepthData { //It's not depth - this needs a better name (so does the BBP for it)
		std::array<PointShadowData, MAX_LIGHTS> depths = {};
	};

	struct LightNumAlignment {
		alignas(16) uint32 numLights;
	};
}