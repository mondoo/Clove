#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/RenderTarget.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::gfx{
	//struct SceneData{
	//
	//	-camera
	//	-lights
	//	-mesh
	//
	// etc
	//
	//} *currentSceneData;

	//Mesh
	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	//Raw data on the lights (without being separated to SBOs)
	PointLightShaderData Renderer::currentLightInfo; //Collects the current light info to bind in
	PointShadowDepthData Renderer::currentShadowDepth; //Collects the current depth info to bind in
	uint32 Renderer::numLights;
	std::array<std::array<math::Matrix4f, 6>, MAX_LIGHTS> Renderer::shadowTransforms = {}; //Each light's shadow transform

	CameraRenderData Renderer::currentCamData;

	std::unique_ptr<MaterialInstance> Renderer::cubeShadowMaterial;

	//Other
	std::shared_ptr<RenderTarget> Renderer::shadowMapRenderTarget; //Render target for the shadow map
	std::shared_ptr<Texture> Renderer::shadowMapTexture; //Texture for the shadow map
	
	std::shared_ptr<RenderTarget> Renderer::customRenderTarget;

	void Renderer::initialise(){
		CLV_LOG_TRACE("Initialising renderer");

		//currentSceneData = new SceneData();

		cubeShadowMaterial = std::make_unique<MaterialInstance>(std::make_shared<Material>(ShaderStyle::CubeShadowMap));

		shadowMapTexture = BindableFactory::createTexture(TBP_Shadow, { TextureStyle::Cubemap, TextureUsage::RenderTarget_Depth, { shadowMapSize, shadowMapSize }, MAX_LIGHTS });
		shadowMapRenderTarget = RenderTarget::createRenderTarget(nullptr, shadowMapTexture.get());

		RenderCommand::setBlendState(true);
	}

	void Renderer::shutDown(){
		CLV_LOG_TRACE("Shutting down renderer");

		//delete currentSceneData;
	}

	void Renderer::beginScene(){
		numLights = 0;
	}

	void Renderer::endScene(){
		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			auto& meshMaterial = mesh->getMaterialInstance();

			//TODO: I like setting them on the material, but I either need a global material or set it on the base material
			//Perhaps some sort of 'material codex' where I can pull the base material for a shader type?

			//Camera
			meshMaterial.setData(BBP_CameraMatrices, ViewData{ currentCamData.lookAt, currentCamData.projection }, ShaderType::Vertex);
			meshMaterial.setData(BBP_ViewData, ViewPos{ currentCamData.position }, ShaderType::Pixel);
			
			//Lights
			meshMaterial.setData(BBP_PointLightData, PointLightShaderData{ currentLightInfo }, ShaderType::Pixel);
			meshMaterial.setData(BBP_CubeDepthData, PointShadowDepthData{ currentShadowDepth }, ShaderType::Pixel);
			meshMaterial.setData(BBP_CurrentLights, LightNumAlignment{ numLights }, ShaderType::Pixel);

			mesh->bind();

			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		const auto generateShadowMap = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			cubeShadowMaterial->bind(); //Bind in the shader / SBOs for generating the cubemap
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		RenderCommand::setDepthBuffer(true);

		//Calculate shadow map
		for(uint32 i = 0; i < numLights; i++){ //TODO: Remove raw loop
			cubeShadowMaterial->setData(BBP_ShadowData, PointShadowShaderData{ shadowTransforms[i] }, ShaderType::Geometry);
			cubeShadowMaterial->setData(BBP_CurrentFaceIndex, LightNumAlignment{ i * 6 }, ShaderType::Geometry);
			cubeShadowMaterial->setData(BBP_CurrentDepthData, PointShadowData{ currentShadowDepth.depths[i] }, ShaderType::Pixel);

			RenderCommand::setViewPortSize(shadowMapSize, shadowMapSize);
			RenderCommand::setRenderTarget(*shadowMapRenderTarget); //TODO: How to set view level????
			std::for_each(meshesToRender.begin(), meshesToRender.end(), generateShadowMap);
			RenderCommand::setViewPortSize(Application::get().getWindow().getWidth(), Application::get().getWindow().getHeight());
		}
		
		RenderCommand::removeCurrentGeometryShader();

		RenderCommand::resetRenderTarget(); //Reset render target before binding the shadow map

		shadowMapTexture->bind(); //Bind this in before rendering the real mesh

		//Render any other render targets
		if(customRenderTarget){
			RenderCommand::setRenderTarget(*customRenderTarget);
			std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);
			RenderCommand::resetRenderTarget();
		}

		//Render scene
		std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);

		RenderCommand::removeTextureAtSlot(TBP_Shadow);

		meshesToRender.clear();
	}

	void Renderer::setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget){
		customRenderTarget = inRenderTarget;
	}

	void Renderer::clearRenderTargets(){
		RenderCommand::setRenderTarget(*shadowMapRenderTarget);
		RenderCommand::clear(); //TODO: Might need to just do a clear depth command
		if(customRenderTarget){
			RenderCommand::setRenderTarget(*customRenderTarget);
			RenderCommand::clear();
		}
		RenderCommand::resetRenderTarget();
		RenderCommand::clear();
	}

	void Renderer::removeRenderTarget(){
		customRenderTarget.reset();
		RenderCommand::resetRenderTarget();
	}

	void Renderer::submitMesh(const std::shared_ptr<Mesh>& mesh){
		meshesToRender.push_back(mesh);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		currentCamData = data;
	}

	void Renderer::submitPointLight(const PointLightData& data){
		const int32 lightIndex = numLights++;
		currentLightInfo.intensities[lightIndex] = data.intensity;

		shadowTransforms[lightIndex] = data.shadowTransforms;

		currentShadowDepth.depths[lightIndex].farPlane = data.farPlane;
		currentShadowDepth.depths[lightIndex].lightPos = data.intensity.position;
	}
}