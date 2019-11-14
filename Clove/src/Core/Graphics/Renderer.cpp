#include "Renderer.hpp"

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/PipelineObject.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/RenderTarget.hpp"
#include "Core/Platform/Application.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/Material.hpp"
#include "Core/Graphics/Renderables/Sprite.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/Graphics/ShaderBufferTypes.hpp"

namespace clv::gfx{
	struct SceneData{
		//VARIABLES
	public:
		CameraRenderData currentCamData;

		std::vector<std::shared_ptr<Mesh>> meshesToRender;

		PointLightShaderData currentLightInfo;
		PointShadowDepthData currentShadowDepth;
		uint32 numLights = 0;
		std::array<std::array<math::Matrix4f, 6>, MAX_LIGHTS> shadowTransforms = {}; 

		MaterialInstance cubeShadowMaterial;

		//Other
		std::shared_ptr<RenderTarget> shadowMapRenderTarget;
		std::shared_ptr<Texture> shadowMapTexture;

		std::shared_ptr<RenderTarget> customRenderTarget;

		std::shared_ptr<PipelineObject> defaultPipeline;
		std::shared_ptr<PipelineObject> shadowPipeline;

		//FUNCTIONS
	public:
		SceneData()
			: cubeShadowMaterial(std::make_shared<Material>()){

			TextureDescriptor tdesc{};
			tdesc.style			= TextureStyle::Cubemap;
			tdesc.usage			= TextureUsage::RenderTarget_Depth;
			tdesc.dimensions	= { shadowMapSize, shadowMapSize };
			tdesc.arraySize		= MAX_LIGHTS;

			shadowMapTexture		= RenderCommand::createTexture(tdesc, nullptr, 4);
			shadowMapRenderTarget	= RenderCommand::createRenderTarget(nullptr, shadowMapTexture.get());

			defaultPipeline = RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::Lit_3D }));
			shadowPipeline	= RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::CubeShadowMap }));
		}

		~SceneData() = default;

		template<typename FunctionPointer>
		void forEachMesh(FunctionPointer&& function){
			std::for_each(meshesToRender.begin(), meshesToRender.end(), function);
		}
	} *currentSceneData = nullptr;

	void Renderer::initialise(){
		CLV_LOG_TRACE("Initialising renderer");
		currentSceneData = new SceneData();
		RenderCommand::setBlendState(true);
	}

	void Renderer::shutDown(){
		CLV_LOG_TRACE("Shutting down renderer");
		delete currentSceneData;
	}

	void Renderer::beginScene(){
		currentSceneData->numLights = 0;
	}

	void Renderer::endScene(){
		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			auto& meshMaterial = mesh->getMaterialInstance();

			//TODO: I like setting them on the material, but I either need a global material or set it on the base material
			//Perhaps some sort of 'material codex' where I can pull the base material for a shader type?

			//Camera
			meshMaterial.setData(BBP_CameraMatrices, ViewData{ currentSceneData->currentCamData.lookAt, currentSceneData->currentCamData.projection }, ShaderType::Vertex);
			meshMaterial.setData(BBP_ViewData, ViewPos{ currentSceneData->currentCamData.position }, ShaderType::Pixel);
			
			//Lights
			meshMaterial.setData(BBP_PointLightData, PointLightShaderData{ currentSceneData->currentLightInfo }, ShaderType::Pixel);
			meshMaterial.setData(BBP_CubeDepthData, PointShadowDepthData{ currentSceneData->currentShadowDepth }, ShaderType::Pixel);
			meshMaterial.setData(BBP_CurrentLights, LightNumAlignment{ currentSceneData->numLights }, ShaderType::Pixel);
			
			meshMaterial.bind();

			const auto vertexLayout = currentSceneData->defaultPipeline->getVertexLayout();

			auto vb = mesh->getVertexBufferForLayout(vertexLayout);
			auto ib = mesh->getIndexBuffer();

			RenderCommand::bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
			RenderCommand::bindIndexBuffer(*ib);

			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		const auto generateShadowMap = [](const std::shared_ptr<Mesh>& mesh){
			const auto vertexLayout = currentSceneData->shadowPipeline->getVertexLayout();

			auto vb = mesh->getVertexBufferForLayout(vertexLayout);
			auto ib = mesh->getIndexBuffer();
			mesh->getMaterialInstance().bind();

			RenderCommand::bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
			RenderCommand::bindIndexBuffer(*ib);

			currentSceneData->cubeShadowMaterial.bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		RenderCommand::setDepthBuffer(true);

		//Calculate shadow map
		RenderCommand::bindPipelineObject(*currentSceneData->shadowPipeline);
		for(uint32 i = 0; i < currentSceneData->numLights; ++i){
			currentSceneData->cubeShadowMaterial.setData(BBP_ShadowData, PointShadowShaderData{ currentSceneData->shadowTransforms[i] }, ShaderType::Geometry);
			currentSceneData->cubeShadowMaterial.setData(BBP_CurrentFaceIndex, LightNumAlignment{ i * 6 }, ShaderType::Geometry);
			currentSceneData->cubeShadowMaterial.setData(BBP_CurrentDepthData, PointShadowData{ currentSceneData->currentShadowDepth.depths[i] }, ShaderType::Pixel);

			RenderCommand::setViewport({ 0, 0, shadowMapSize, shadowMapSize });
			RenderCommand::setRenderTarget(*currentSceneData->shadowMapRenderTarget);
			currentSceneData->forEachMesh(generateShadowMap);
			RenderCommand::setViewport({ 0, 0, plt::Application::get().getWindow().getWidth(), plt::Application::get().getWindow().getHeight() });
		}

		RenderCommand::resetRenderTargetToDefault(); //Reset render target before binding the shadow map

		RenderCommand::bindPipelineObject(*currentSceneData->defaultPipeline); //Bind in the default pipeline
		RenderCommand::bindTexture(currentSceneData->shadowMapTexture.get(), TBP_Shadow); //Bind this in before rendering the real mesh

		//Render any other render targets
		if(currentSceneData->customRenderTarget){
			RenderCommand::setRenderTarget(*currentSceneData->customRenderTarget);
			currentSceneData->forEachMesh(draw);
			RenderCommand::resetRenderTargetToDefault();
		}

		//Render scene
		currentSceneData->forEachMesh(draw);

		RenderCommand::bindTexture(nullptr, TBP_Shadow);

		currentSceneData->meshesToRender.clear();
	}

	void Renderer::setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget){
		currentSceneData->customRenderTarget = inRenderTarget;
	}

	void Renderer::clearRenderTargets(){
		RenderCommand::setRenderTarget(*currentSceneData->shadowMapRenderTarget);
		RenderCommand::clear(); //TODO: Might need to just do a clear depth command
		if(currentSceneData->customRenderTarget){
			RenderCommand::setRenderTarget(*currentSceneData->customRenderTarget);
			RenderCommand::clear();
		}
		RenderCommand::resetRenderTargetToDefault();
		RenderCommand::clear();
	}

	void Renderer::removeRenderTarget(){
		currentSceneData->customRenderTarget.reset();
		RenderCommand::resetRenderTargetToDefault();
	}

	void Renderer::submitMesh(const std::shared_ptr<Mesh>& mesh){
		currentSceneData->meshesToRender.push_back(mesh);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		currentSceneData->currentCamData = data;
	}

	void Renderer::submitPointLight(const PointLightData& data){
		const int32 lightIndex = currentSceneData->numLights++;
		currentSceneData->currentLightInfo.intensities[lightIndex] = data.intensity;

		currentSceneData->shadowTransforms[lightIndex] = data.shadowTransforms;

		currentSceneData->currentShadowDepth.depths[lightIndex].farPlane = data.farPlane;
		currentSceneData->currentShadowDepth.depths[lightIndex].lightPos = data.intensity.position;
	}
}