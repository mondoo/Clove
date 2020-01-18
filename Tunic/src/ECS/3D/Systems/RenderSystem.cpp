#include "Tunic/ECS/3D/Systems/RenderSystem.hpp"

#include "Tunic/ECS/Core/Manager.hpp"
#include "Tunic/Application.hpp"
#include "Tunic/ECS/3D/Components/TransformComponent.hpp"
#include "Tunic/ECS/3D/Components/MeshComponent.hpp"
#include "Tunic/ECS/3D/Components/CameraComponent.hpp"
#include "Tunic/ECS/3D/Components/LightComponent.hpp"
#include "Tunic/Rendering/Renderables/Mesh.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/CommandBuffer.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"
#include "Clove/Graphics/Core/PipelineObject.hpp"
#include "Clove/Platform/Core/Window.hpp"

using namespace clv;
using namespace clv::gfx;
using namespace tnc::rnd;

namespace tnc::ecs::_3D{
	RenderSystem::SceneData::SceneData() = default;

	RenderSystem::SceneData::~SceneData() = default;

	RenderSystem::RenderSystem()
		: cubeShadowMaterial(std::make_shared<Material>()){
		windowCommandBuffer = Application::get().getGraphicsFactory().createCommandBuffer(Application::get().getMainWindow().getSurface());
	
		TextureDescriptor tdesc{};
		tdesc.style			= TextureStyle::Cubemap;
		tdesc.usage			= TextureUsage::RenderTarget_Depth;
		tdesc.dimensions	= { shadowMapSize, shadowMapSize };
		tdesc.arraySize		= MAX_LIGHTS;

		shadowMapTexture = Application::get().getGraphicsFactory().createTexture(tdesc, nullptr, 4);
		auto shadowMapRenderTarget = Application::get().getGraphicsFactory().createRenderTarget(nullptr, shadowMapTexture.get());
		shadowMapCommandBuffer = Application::get().getGraphicsFactory().createCommandBuffer(shadowMapRenderTarget);

		defaultPipeline = Application::get().getGraphicsFactory().createPipelineObject(Application::get().getGraphicsFactory().createShader({ ShaderStyle::Lit_3D }));
		shadowPipeline = Application::get().getGraphicsFactory().createPipelineObject(Application::get().getGraphicsFactory().createShader({ ShaderStyle::CubeShadowMap }));
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept
		: cubeShadowMaterial(other.cubeShadowMaterial){
		sceneData = other.sceneData;

		windowCommandBuffer = other.windowCommandBuffer;
		shadowMapCommandBuffer = other.shadowMapCommandBuffer;
		shadowMapTexture = other.shadowMapTexture;

		defaultPipeline = other.defaultPipeline;
		shadowPipeline = other.shadowPipeline;
	}

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept{
		sceneData = other.sceneData;

		windowCommandBuffer = other.windowCommandBuffer;
		shadowMapCommandBuffer = other.shadowMapCommandBuffer;
		shadowMapTexture = other.shadowMapTexture;
		cubeShadowMaterial = other.cubeShadowMaterial;

		defaultPipeline = other.defaultPipeline;
		shadowPipeline = other.shadowPipeline;

		return *this;
	}

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::preUpdate(){
		CLV_PROFILE_FUNCTION();

		sceneData.numLights = 0;
		sceneData.cameras.clear();
		sceneData.meshesToRender.clear();
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
				const mth::quatf cameraRotation = transform->getRotation();

				mth::vec3f eulerRot = mth::quaternionToEuler(cameraRotation);

				mth::vec3f front;
				front.x = sin(eulerRot.y) * cos(eulerRot.x);
				front.y = sin(eulerRot.x);
				front.z = cos(eulerRot.y) * cos(eulerRot.x);

				camera->cameraFront = mth::normalise(front);
				camera->cameraRight = mth::normalise(mth::cross(camera->cameraFront, mth::vec3f{ 0.0f, 1.0f, 0.0f }));
				camera->cameraUp	= mth::normalise(mth::cross(camera->cameraRight, camera->cameraFront));

				const mth::mat4f lookAt = mth::lookAt(position, position + camera->cameraFront, camera->cameraUp);

				camera->cameraRenderData.lookAt 	= lookAt;
				camera->cameraRenderData.position 	= position;
				camera->cameraRenderData.projection = camera->currentProjection;

				sceneData.cameras.push_back({ camera->viewport, camera->cameraRenderData, camera->renderTarget });
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

				sceneData.meshesToRender.push_back(renderable->mesh);
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

				const int32 lightIndex = sceneData.numLights++;
				sceneData.currentLightInfo.intensities[lightIndex] = light->lightData.intensity;

				sceneData.shadowTransforms[lightIndex] = light->lightData.shadowTransforms;

				sceneData.currentShadowDepth.depths[lightIndex].farPlane = light->lightData.farPlane;
				sceneData.currentShadowDepth.depths[lightIndex].lightPos = light->lightData.intensity.position;
			}
		}
	}

	void RenderSystem::postUpdate(){
		CLV_PROFILE_FUNCTION();

		windowCommandBuffer->beginEncoding();
		shadowMapCommandBuffer->beginEncoding();

		const auto renderCamera = [this](const ComposedCameraData& cameraData){
			//TODO: Need a better system than this
			auto& commandBuffer = cameraData.target ? Application::get().getGraphicsFactory().createCommandBuffer(cameraData.target) : windowCommandBuffer; 
			if(cameraData.target){
				commandBuffer->beginEncoding();
			}

			const auto draw = [this, camBufferData = cameraData.bufferData, commandBuffer](const std::shared_ptr<Mesh>& mesh){
				auto& meshMaterial = mesh->getMaterialInstance();

				//TODO: I like setting them on the material, but I either need a global material or set it on the base material
				//Perhaps some sort of 'material codex' where I can pull the base material for a shader type?

				//Camera
				meshMaterial.setData(BBP_CameraMatrices, ViewData{ camBufferData.lookAt, camBufferData.projection }, ShaderType::Vertex);
				meshMaterial.setData(BBP_ViewData, ViewPos{ camBufferData.position }, ShaderType::Pixel);

				//Lights
				meshMaterial.setData(BBP_PointLightData, PointLightShaderData{ sceneData.currentLightInfo }, ShaderType::Pixel);
				meshMaterial.setData(BBP_CubeDepthData, PointShadowDepthData{ sceneData.currentShadowDepth }, ShaderType::Pixel);
				meshMaterial.setData(BBP_CurrentLights, LightNumAlignment{ sceneData.numLights }, ShaderType::Pixel);

				meshMaterial.bind(commandBuffer);

				const auto vertexLayout = defaultPipeline->getVertexLayout();

				auto vb = mesh->getVertexBufferForLayout(vertexLayout);
				auto ib = mesh->getIndexBuffer();

				commandBuffer->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				commandBuffer->bindIndexBuffer(*ib);

				commandBuffer->drawIndexed(mesh->getIndexCount());
			};

			commandBuffer->setViewport(cameraData.viewport);

			commandBuffer->bindPipelineObject(*defaultPipeline); //Bind in the default pipeline
			commandBuffer->bindTexture(shadowMapTexture.get(), TBP_Shadow); //Bind this in before rendering the real mesh

			//Render scene
			sceneData.forEachMesh(draw);

			commandBuffer->bindTexture(nullptr, TBP_Shadow);

			if(cameraData.target){
				commandBuffer->flushCommands();
			}
		};

		const auto generateShadowMap = [this](const std::shared_ptr<Mesh>& mesh){
			const auto vertexLayout = shadowPipeline->getVertexLayout();

			auto vb = mesh->getVertexBufferForLayout(vertexLayout);
			auto ib = mesh->getIndexBuffer();
			mesh->getMaterialInstance().bind(shadowMapCommandBuffer);

			shadowMapCommandBuffer->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
			shadowMapCommandBuffer->bindIndexBuffer(*ib);

			cubeShadowMaterial.bind(shadowMapCommandBuffer);
			shadowMapCommandBuffer->drawIndexed(mesh->getIndexCount());
		};

		windowCommandBuffer->setDepthEnabled(true);

		//Calculate shadow map
		shadowMapCommandBuffer->bindPipelineObject(*shadowPipeline);
		shadowMapCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });
		for(uint32 i = 0; i < sceneData.numLights; ++i){
			//TODO: Will probably cause issues with the command buffer
			cubeShadowMaterial.setData(BBP_ShadowData, PointShadowShaderData{ sceneData.shadowTransforms[i] }, ShaderType::Geometry);
			cubeShadowMaterial.setData(BBP_CurrentFaceIndex, LightNumAlignment{ i * 6 }, ShaderType::Geometry);
			cubeShadowMaterial.setData(BBP_CurrentDepthData, PointShadowData{ sceneData.currentShadowDepth.depths[i] }, ShaderType::Pixel);

			sceneData.forEachMesh(generateShadowMap);
		}
		shadowMapCommandBuffer->flushCommands();

		//Render scene for each camera
		std::for_each(sceneData.cameras.begin(), sceneData.cameras.end(), renderCamera);

		windowCommandBuffer->flushCommands();
	}
}