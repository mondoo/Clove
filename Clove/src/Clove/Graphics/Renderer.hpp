#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

#include <queue>

namespace clv::gfx{
	class RenderTarget;
	class Mesh;
	class Sprite;
	class Texture;

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


	struct PointLightIntesity{
		alignas(16) math::Vector3f position = { };

		alignas(16) math::Vector3f ambient = { };
		alignas(16) math::Vector3f diffuse = { };
		alignas(16) math::Vector3f specular = { };

		/*alignas(16)*/ float constant = 0;
		/*alignas(16)*/ float linear = 0;
		/*alignas(16)*/ float quadratic = 0;
	};
	struct PointLightData{
		PointLightIntesity intensity = { };
		std::array<math::Matrix4f, 6> shadowTransforms = { };
	};
	struct PointLightShaderData{ //I guess the lighting system could handle this
		int32 numLights = 0;
		std::array<PointLightIntesity, 10> intensities = { };
	};
	struct PointShadowShaderData{
		int32 numLights = 0;
		std::array<std::array<math::Matrix4f, 6>, 10> shadowTransforms = { };
	};

	/*
	STEPS TO DO NEXT:
	-Create the shader for generating the cube map
	-Update render pipeline to use this shader instead of the mesh's shader (not sure how DX will handle it)
	-Update lit shader to take in the cube map and then do the depth testing

	*/

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