#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/GraphicsConstants.hpp"

namespace clv::gfx{
	class RenderTarget;
	class Mesh;
	class Sprite;
	class Texture;
	class Shader;

	struct MaterialData{
		alignas(16) float sininess;
	};

	struct ViewData{
		math::Matrix4f view;
		math::Matrix4f projection;
	};

	struct ViewPos{
		alignas(16) math::Vector3f pos;
	};

	struct CameraRenderData{
		math::Vector3f position;
		math::Matrix4f lookAt;
		math::Matrix4f projection;
	};

	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct PointLightIntesity{
		math::Vector3f position = { };
		float constant = 0;
		math::Vector3f ambient = { };
		float linear = 0;
		math::Vector3f diffuse = { };
		float quadratic = 0;
		alignas(16) math::Vector3f specular = { };
	};
	struct PointLightData{
		PointLightIntesity intensity = { };
		std::array<math::Matrix4f, 6> shadowTransforms = { };
		float farPlane;
	};
	struct PointLightShaderData{
		std::array<PointLightIntesity, MAX_LIGHTS> intensities = {};
	};
	struct PointShadowShaderData{
		//int32 numLights = 0;
		//std::array<std::array<math::Matrix4f, 6>, 10> shadowTransforms = { };
		std::array<math::Matrix4f, 6> shadowTransforms = { }; //Supporting one for now
	};
	struct PointShadowData{
		math::Vector3f lightPos = { };
		float farPlane = 0;
	};
	struct PointShadowDepthData{ //It's not depth - this needs a better name
		std::array<PointShadowData, MAX_LIGHTS> depths = {};
	};

	struct LightNumAlignment{
		alignas(16) uint32 numLights;
	};

	class Renderer{
		//VARIABLES
	public:
		//TODO: Is there a better place to encapsulate this?
		static constexpr uint32 shadowMapSize = 1024; //arbitrary shadowmap size

	private:
		static std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> materialSBO; //TODO: MOVE TO MATERIAL

		static std::shared_ptr<gfx::ShaderBufferObject<ViewData>> viewDataSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> viewPosSBO;

		static std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> lightDataSBO;
		static PointLightShaderData currentLightInfo;
		static std::shared_ptr<gfx::ShaderBufferObject<PointShadowShaderData>> shadowDataSBO;
		static PointShadowShaderData currentShadowInfo;
		static std::shared_ptr<gfx::ShaderBufferObject<PointShadowDepthData>> shadowDepthData;
		static PointShadowDepthData currentShadowDepth;
		static std::shared_ptr<gfx::ShaderBufferObject<VertexData>> shadowModelData;
		static std::shared_ptr<gfx::Shader> cubeShadowMapShader;
		static std::shared_ptr<gfx::ShaderBufferObject<LightNumAlignment>> lightNumSBO;
		static uint32 numLights;

		static std::vector<std::shared_ptr<Mesh>> meshesToRender;

		static CameraRenderData cameraSubmissionData;

		static std::shared_ptr<RenderTarget> shadowMapRenderTarget;
		static std::shared_ptr<Texture> shadowMapTexture;

		static std::shared_ptr<RenderTarget> customRenderTarget;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene(); //How would I begin scene with the ecs?
		static void endScene();

		static void setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget);
		static void clearRenderTargets();
		static void removeRenderTarget();

		static void submitMesh(const std::shared_ptr<Mesh>& mesh);
		static void setCamera(const CameraRenderData& data);
		static void submitPointLight(const PointLightData& data);
	};
}