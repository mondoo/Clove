#include "Tunic/ECS/3D/Systems/RenderSystem.hpp"

#include "Tunic/ECS/Core/Manager.hpp"
#include "Tunic/ECS/3D/Components/TransformComponent.hpp"
#include "Tunic/ECS/3D/Components/MeshComponent.hpp"
#include "Tunic/ECS/3D/Components/CameraComponent.hpp"
#include "Tunic/ECS/3D/Components/LightComponent.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"
#include "Clove/Core/Graphics/GraphicsGlobal.hpp"
#include "Clove/Core/Graphics/PipelineObject.hpp"
#include "Clove/Core/Graphics/Resources/Texture.hpp"
#include "Clove/Core/Graphics/ShaderBufferTypes.hpp"
#include "Clove/Core/Graphics/Renderables/Mesh.hpp"
#include "Clove/Core/Graphics/MaterialInstance.hpp"
#include "Clove/Core/Platform/Window.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::ecs::_3D{
	struct ComposedCameraData{
		Viewport viewport;
		CameraRenderData bufferData;
		std::shared_ptr<RenderTarget> target;
	};

	struct SceneData{
		//VARIABLES
	public:
		std::vector<ComposedCameraData> cameras;

		std::vector<std::shared_ptr<Mesh>> meshesToRender;

		PointLightShaderData currentLightInfo;
		PointShadowDepthData currentShadowDepth;
		uint32 numLights = 0;
		std::array<std::array<mth::mat4f, 6>, MAX_LIGHTS> shadowTransforms = {};

		MaterialInstance cubeShadowMaterial;

		//Other
		std::shared_ptr<RenderTarget> shadowMapRenderTarget;
		std::shared_ptr<Texture> shadowMapTexture;

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

			shadowMapTexture		= global::graphicsFactory->createTexture(tdesc, nullptr, 4);
			shadowMapRenderTarget	= global::graphicsFactory->createRenderTarget(nullptr, shadowMapTexture.get());

			defaultPipeline = global::graphicsFactory->createPipelineObject(global::graphicsFactory->createShader({ ShaderStyle::Lit_3D }));
			shadowPipeline	= global::graphicsFactory->createPipelineObject(global::graphicsFactory->createShader({ ShaderStyle::CubeShadowMap }));
		}

		~SceneData() = default;

		template<typename FunctionPointer>
		void forEachMesh(FunctionPointer&& function){
			std::for_each(meshesToRender.begin(), meshesToRender.end(), function);
		}
	} *currentSceneData = nullptr;

	RenderSystem::RenderSystem(){
		CLV_LOG_TRACE("Initialising renderer");
		currentSceneData = new SceneData();
		global::graphicsDevice->setBlendState(true);
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem(){
		CLV_LOG_TRACE("Shutting down renderer");
		delete currentSceneData;
	}

	void RenderSystem::preUpdate(){
		CLV_PROFILE_FUNCTION();

		global::graphicsDevice->setRenderTarget(currentSceneData->shadowMapRenderTarget.get());
		global::graphicsDevice->clear(); //TODO: Might need to just do a clear depth command
		std::for_each(currentSceneData->cameras.begin(), currentSceneData->cameras.end(), [](const ComposedCameraData& cameraData){
			if(cameraData.target){
				global::graphicsDevice->setRenderTarget(cameraData.target.get());
				global::graphicsDevice->clear();
			}
		});
		global::graphicsDevice->resetRenderTargetToDefault();
		global::graphicsDevice->clear();

		currentSceneData->numLights = 0;
		currentSceneData->cameras.clear();
		currentSceneData->meshesToRender.clear();
	}

	void RenderSystem::update(utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		//Set camera
		{
			auto componentTuples = manager->getComponentSets<TransformComponent, CameraComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				CameraComponent* camera = std::get<CameraComponent*>(tuple);

				const mth::vec3f& position = transform->getPosition();

				//update front
				const mth::quatf cameraRotation = transform->getRotation();
				mth::vec3f eulerRot = mth::quaternionToEuler(cameraRotation);

				if(eulerRot.x >= mth::pi<float>){ //This stops it moving the other way
					eulerRot.y -= mth::pi<float>;
				}

				mth::vec3f front;
				front.x = sin(eulerRot.y) * cos(eulerRot.x);
				front.y = sin(eulerRot.y) * sin(eulerRot.x);
				front.z = cos(eulerRot.y);
				camera->cameraFront = mth::normalise(front);

				//update look at
				const mth::mat4f lookAt = mth::lookAt(position, position + camera->cameraFront, camera->cameraUp);

				//update right
				camera->cameraRight = mth::normalise(mth::cross(camera->cameraFront, camera->cameraUp));

				camera->cameraRenderData.lookAt 	= lookAt;
				camera->cameraRenderData.position 	= position;
				camera->cameraRenderData.projection = camera->currentProjection;

				currentSceneData->cameras.push_back({ camera->viewport, camera->cameraRenderData, camera->renderTarget });
			}
		}

		//Submit meshes
		{
			auto componentTuples = manager->getComponentSets<TransformComponent, MeshComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				MeshComponent* renderable = std::get<MeshComponent*>(tuple);

				const mth::mat4f model = transform->getWorldTransformMatrix();
				renderable->mesh->getMaterialInstance().setData(BBP_ModelData, VertexData{ model, mth::transpose(mth::inverse(model)) }, ShaderType::Vertex);

				currentSceneData->meshesToRender.push_back(renderable->mesh);
			}
		}

		//Submit lights
		{
			auto componentTuples = manager->getComponentSets<TransformComponent, LightComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				LightComponent* light = std::get<LightComponent*>(tuple);

				const auto& position = transform->getPosition();

				light->lightData.intensity.position = position;
				light->lightData.shadowTransforms[0] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 1.0, 0.0, 0.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[1] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ -1.0, 0.0, 0.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[2] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 1.0, 0.0 }, mth::vec3f{ 0.0, 0.0, 1.0 });
				light->lightData.shadowTransforms[3] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0,-1.0, 0.0 }, mth::vec3f{ 0.0, 0.0,-1.0 });
				light->lightData.shadowTransforms[4] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 0.0, 1.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[5] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 0.0,-1.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });

				const int32 lightIndex = currentSceneData->numLights++;
				currentSceneData->currentLightInfo.intensities[lightIndex] = light->lightData.intensity;

				currentSceneData->shadowTransforms[lightIndex] = light->lightData.shadowTransforms;

				currentSceneData->currentShadowDepth.depths[lightIndex].farPlane = light->lightData.farPlane;
				currentSceneData->currentShadowDepth.depths[lightIndex].lightPos = light->lightData.intensity.position;
			}
		}
	}

	void RenderSystem::postUpdate(){
		CLV_PROFILE_FUNCTION();

		const auto renderCamera = [](const ComposedCameraData& cameraData){
			const auto draw = [camBufferData = cameraData.bufferData](const std::shared_ptr<Mesh>& mesh){
				auto& meshMaterial = mesh->getMaterialInstance();

				//TODO: I like setting them on the material, but I either need a global material or set it on the base material
				//Perhaps some sort of 'material codex' where I can pull the base material for a shader type?

				//Camera
				meshMaterial.setData(BBP_CameraMatrices, ViewData{ camBufferData.lookAt, camBufferData.projection }, ShaderType::Vertex);
				meshMaterial.setData(BBP_ViewData, ViewPos{ camBufferData.position }, ShaderType::Pixel);

				//Lights
				meshMaterial.setData(BBP_PointLightData, PointLightShaderData{ currentSceneData->currentLightInfo }, ShaderType::Pixel);
				meshMaterial.setData(BBP_CubeDepthData, PointShadowDepthData{ currentSceneData->currentShadowDepth }, ShaderType::Pixel);
				meshMaterial.setData(BBP_CurrentLights, LightNumAlignment{ currentSceneData->numLights }, ShaderType::Pixel);

				meshMaterial.bind();

				const auto vertexLayout = currentSceneData->defaultPipeline->getVertexLayout();

				auto vb = mesh->getVertexBufferForLayout(vertexLayout);
				auto ib = mesh->getIndexBuffer();

				global::graphicsDevice->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				global::graphicsDevice->bindIndexBuffer(*ib);

				global::graphicsDevice->drawIndexed(mesh->getIndexCount());
			};

			global::graphicsDevice->setViewport(cameraData.viewport);

			if(cameraData.target){
				global::graphicsDevice->setRenderTarget(cameraData.target.get());
			} else{
				global::graphicsDevice->resetRenderTargetToDefault();
			}

			global::graphicsDevice->bindPipelineObject(*currentSceneData->defaultPipeline); //Bind in the default pipeline
			global::graphicsDevice->bindTexture(currentSceneData->shadowMapTexture.get(), TBP_Shadow); //Bind this in before rendering the real mesh

			//Render scene
			currentSceneData->forEachMesh(draw);

			global::graphicsDevice->bindTexture(nullptr, TBP_Shadow);
		};

		const auto generateShadowMap = [](const std::shared_ptr<Mesh>& mesh){
			const auto vertexLayout = currentSceneData->shadowPipeline->getVertexLayout();

			auto vb = mesh->getVertexBufferForLayout(vertexLayout);
			auto ib = mesh->getIndexBuffer();
			mesh->getMaterialInstance().bind();

			global::graphicsDevice->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
			global::graphicsDevice->bindIndexBuffer(*ib);

			currentSceneData->cubeShadowMaterial.bind();
			global::graphicsDevice->drawIndexed(mesh->getIndexCount());
		};

		global::graphicsDevice->setDepthBuffer(true);

		//Calculate shadow map
		global::graphicsDevice->bindPipelineObject(*currentSceneData->shadowPipeline);
		global::graphicsDevice->setViewport({ 0, 0, shadowMapSize, shadowMapSize });
		for(uint32 i = 0; i < currentSceneData->numLights; ++i){
			currentSceneData->cubeShadowMaterial.setData(BBP_ShadowData, PointShadowShaderData{ currentSceneData->shadowTransforms[i] }, ShaderType::Geometry);
			currentSceneData->cubeShadowMaterial.setData(BBP_CurrentFaceIndex, LightNumAlignment{ i * 6 }, ShaderType::Geometry);
			currentSceneData->cubeShadowMaterial.setData(BBP_CurrentDepthData, PointShadowData{ currentSceneData->currentShadowDepth.depths[i] }, ShaderType::Pixel);

			global::graphicsDevice->setRenderTarget(currentSceneData->shadowMapRenderTarget.get());
			currentSceneData->forEachMesh(generateShadowMap);
		}

		//Render scene for each camera
		std::for_each(currentSceneData->cameras.begin(), currentSceneData->cameras.end(), renderCamera);
	}
}