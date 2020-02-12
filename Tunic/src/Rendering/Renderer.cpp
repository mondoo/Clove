#include "Tunic/Rendering/Renderer.hpp"

#include "Tunic/Rendering/RenderingConstants.hpp"
#include "Tunic/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/Core/GraphicsFactory.hpp>
#include <Clove/Graphics/Core/CommandBuffer.hpp>
#include <Clove/Graphics/Core/Surface.hpp>
#include <Clove/Graphics/Core/RenderTarget.hpp>
#include <Clove/Graphics/Core/PipelineObject.hpp>
#include <Clove/Graphics/Core/Resources/Texture.hpp>

#include <Clove/Platform/Core/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd{
	Renderer::SceneData::SceneData() = default;

	Renderer::SceneData::~SceneData() = default;

	Renderer::Renderer(plt::Window& window)
		: surface(window.getSurface()){

		GraphicsFactory& factory = window.getGraphicsFactory();
		
		//Shadow map
		gfx::TextureDescriptor tdesc{};
		tdesc.style			= TextureStyle::Cubemap;
		tdesc.usage			= TextureUsage::RenderTarget_Depth;
		tdesc.dimensions	= { shadowMapSize, shadowMapSize };
		tdesc.arraySize		= MAX_LIGHTS;

		shadowMapTexture	= factory.createTexture(tdesc, nullptr, 4);
		shadowRenderTarget	= factory.createRenderTarget(nullptr, shadowMapTexture.get());
		shadowCommandBuffer = factory.createCommandBuffer();

		auto shadowVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/CubeShadowMap-vs.hlsl");
		auto shadowGS = factory.createShader({ ShaderStage::Geometry }, "res/Shaders/CubeShadowMap-gs.hlsl");
		auto shadowPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/CubeShadowMap-ps.hlsl");
		shadowPipelineObject = factory.createPipelineObject();
		shadowPipelineObject->setVertexShader(*shadowVS);
		shadowPipelineObject->setGeometryShader(*shadowGS);
		shadowPipelineObject->setPixelShader(*shadowPS);

		//Mesh
		meshCommandBuffer = factory.createCommandBuffer();

		auto meshVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/Lit-vs.hlsl");
		auto meshPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/Lit-ps.hlsl");
		meshPipelineObject = factory.createPipelineObject();
		meshPipelineObject->setVertexShader(*meshVS);
		meshPipelineObject->setPixelShader(*meshPS);

		//Sprites
		//spriteCommandBuffer = factory.createCommandBuffer();

		//auto spriteVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/2D-vs.hlsl");
		//auto spritePS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/2D-ps.hlsl");
		//spritePipelineObject = factory.createPipelineObject();
		//spritePipelineObject->setVertexShader(*spriteVS);
		//spritePipelineObject->setPixelShader(*spritePS);

		//Fonts
		//TODO:
	}

	void Renderer::begin(){
		CLV_PROFILE_FUNCTION();

		scene.meshes.clear();
		scene.numLights = 0;
		scene.cameras.clear();
		
		//sceneData.widgetsToRender.clear();
		//sceneData.charactersToRender.clear();


		
	}

	void Renderer::submitMesh(const std::shared_ptr<rnd::Mesh>& mesh){
		scene.meshes.push_back(mesh);
	}

	void Renderer::submitLight(const PointLightData& light){
		const int32_t lightIndex = scene.numLights++;

		scene.currentLightInfo.intensities[lightIndex] = light.intensity;

		scene.shadowTransforms[lightIndex] = light.shadowTransforms;

		scene.currentShadowDepth.depths[lightIndex].farPlane = light.farPlane;
		scene.currentShadowDepth.depths[lightIndex].lightPos = light.intensity.position;
	}

	void Renderer::submitCamera(const ComposedCameraData& camera){
		scene.cameras.push_back(camera);
	}

	void Renderer::submitSprite(const Sprite& sprite){

	}

	void Renderer::end(){
		CLV_PROFILE_FUNCTION();

		shadowCommandBuffer->beginEncoding(shadowRenderTarget);
		//spriteCommandBuffer->beginEncoding(surface->getRenderTarget());

		meshCommandBuffer->clearTarget(); //Sprite is cleared here
		shadowCommandBuffer->clearTarget();

		meshCommandBuffer->setDepthEnabled(true);
		meshCommandBuffer->bindPipelineObject(*meshPipelineObject);
		meshCommandBuffer->bindTexture(shadowMapTexture.get(), TBP_Shadow);

		shadowCommandBuffer->setDepthEnabled(true);
		shadowCommandBuffer->bindPipelineObject(*shadowPipelineObject);
		shadowCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });

		//Renderpass per camera
		for (auto& camera : scene.cameras){
			if (camera.target){
				meshCommandBuffer->beginEncoding(camera.target);
			} else{
				meshCommandBuffer->beginEncoding(surface->getRenderTarget());
			}

			meshCommandBuffer->setViewport(camera.viewport);

			const CameraRenderData& camRenderData = camera.bufferData;

			for (auto& mesh : scene.meshes){
				auto& meshMaterial = mesh->getMaterialInstance();

				//Camera
				meshMaterial.setData(BBP_CameraMatrices, ViewData{ camRenderData.lookAt, camRenderData.projection }, ShaderStage::Vertex);
				meshMaterial.setData(BBP_ViewData, ViewPos{ camRenderData.position }, ShaderStage::Pixel);

				//Lights
				meshMaterial.setData(BBP_PointLightData, PointLightShaderData{ scene.currentLightInfo }, ShaderStage::Pixel);
				meshMaterial.setData(BBP_CubeDepthData, PointShadowDepthData{ scene.currentShadowDepth }, ShaderStage::Pixel);
				meshMaterial.setData(BBP_CurrentLights, LightNumAlignment{ scene.numLights }, ShaderStage::Pixel);

				meshMaterial.bind(meshCommandBuffer);

				const VertexLayout& vertexLayout = meshPipelineObject->getVertexLayout();
				mesh->bind(*meshCommandBuffer, vertexLayout);

				meshCommandBuffer->drawIndexed(mesh->getIndexCount());
			}
		}

		meshCommandBuffer->bindTexture(nullptr, TBP_Shadow);

		//Renderpass per light
		std::vector<MaterialInstance> capturedMaterials;
		for (uint32_t i = 0; i < scene.numLights; ++i){
			for (auto& mesh : scene.meshes){
				auto meshMaterial = mesh->getMaterialInstance();
				meshMaterial.setData(BBP_ShadowData, PointShadowShaderData{ scene.shadowTransforms[i] }, ShaderStage::Geometry);
				meshMaterial.setData(BBP_CurrentFaceIndex, LightNumAlignment{ i * 6 }, ShaderStage::Geometry);
				meshMaterial.setData(BBP_CurrentDepthData, PointShadowData{ scene.currentShadowDepth.depths[i] }, ShaderStage::Pixel);

				meshMaterial.bind(shadowCommandBuffer);

				const VertexLayout& vertexLayout = shadowPipelineObject->getVertexLayout();
				mesh->bind(*shadowCommandBuffer, vertexLayout);

				shadowCommandBuffer->drawIndexed(mesh->getIndexCount());

				capturedMaterials.push_back(meshMaterial);
			}
		}

		//End encoding in specific order
		shadowCommandBuffer->endEncoding();
		meshCommandBuffer->endEncoding();
		//spriteCommandBuffer->endEncoding();
	}
}