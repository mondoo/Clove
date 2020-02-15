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

	Renderer::Renderer(plt::Window& window){

		GraphicsFactory& factory = window.getGraphicsFactory();
		
		//Mesh command buffer
		meshCommandBuffer = factory.createCommandBuffer();

		auto meshVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/Lit-vs.hlsl");
		auto meshPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/Lit-ps.hlsl");
		meshPipelineObject = factory.createPipelineObject();
		meshPipelineObject->setVertexShader(*meshVS);
		meshPipelineObject->setPixelShader(*meshPS);

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

		//Buffers
		gfx::BufferDescriptor bufferDesc{};
		bufferDesc.elementSize	= 0;
		bufferDesc.bufferType	= clv::gfx::BufferType::ShaderResourceBuffer;
		bufferDesc.bufferUsage	= clv::gfx::BufferUsage::Dynamic;

		bufferDesc.bufferSize = sizeof(ViewData);
		viewBuffer = factory.createBuffer(bufferDesc, nullptr);
		bufferDesc.bufferSize = sizeof(ViewPos);
		viewPosition = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(PointLightShaderData);
		lightInfoBuffer = factory.createBuffer(bufferDesc, nullptr);
		bufferDesc.bufferSize = sizeof(PointShadowDepthData);
		lightDepthBuffer = factory.createBuffer(bufferDesc, nullptr);
		bufferDesc.bufferSize = sizeof(LightNumAlignment);
		lightNumBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(PointShadowShaderData);
		shadowInfoBuffer = factory.createBuffer(bufferDesc, nullptr);
		bufferDesc.bufferSize = sizeof(LightNumAlignment);
		lightIndexBuffer = factory.createBuffer(bufferDesc, nullptr);
		bufferDesc.bufferSize = sizeof(PointShadowData);
		shadowLightPosBuffer = factory.createBuffer(bufferDesc, nullptr);

	}

	void Renderer::begin(){
		CLV_PROFILE_FUNCTION();

		scene.meshes.clear();
		scene.numLights = 0;
		scene.cameras.clear();	
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

	void Renderer::end(){
		CLV_PROFILE_FUNCTION();
	
		//Draw all meshes in the scene
		meshCommandBuffer->setDepthEnabled(true);
		meshCommandBuffer->bindPipelineObject(*meshPipelineObject);
		meshCommandBuffer->bindTexture(shadowMapTexture.get(), TBP_Shadow);

		meshCommandBuffer->updateBufferData(*lightInfoBuffer, &scene.currentLightInfo);
		meshCommandBuffer->bindShaderResourceBuffer(*lightInfoBuffer, ShaderStage::Pixel, BBP_PointLightData);

		meshCommandBuffer->updateBufferData(*lightDepthBuffer, &scene.currentShadowDepth);
		meshCommandBuffer->bindShaderResourceBuffer(*lightDepthBuffer, ShaderStage::Pixel, BBP_CubeDepthData);

		auto numLights = LightNumAlignment{ scene.numLights };
		meshCommandBuffer->updateBufferData(*lightNumBuffer, &numLights);
		meshCommandBuffer->bindShaderResourceBuffer(*lightNumBuffer, ShaderStage::Pixel, BBP_CurrentLights);

		for (auto& camera : scene.cameras){
			meshCommandBuffer->beginEncoding(camera.target);

			meshCommandBuffer->setViewport(camera.viewport);

			const CameraRenderData& camRenderData = camera.bufferData;

			auto viewData = ViewData{ camRenderData.lookAt, camRenderData.projection };
			meshCommandBuffer->updateBufferData(*viewBuffer, &viewData);
			meshCommandBuffer->bindShaderResourceBuffer(*viewBuffer, ShaderStage::Vertex, BBP_CameraMatrices);

			auto viewPos = ViewPos{ camRenderData.position };
			meshCommandBuffer->updateBufferData(*viewPosition, &viewPos);
			meshCommandBuffer->bindShaderResourceBuffer(*viewPosition, ShaderStage::Pixel, BBP_ViewData);

			for (auto& mesh : scene.meshes){
				mesh->draw(*meshCommandBuffer, meshPipelineObject->getVertexLayout());
			}
		}

		meshCommandBuffer->bindTexture(nullptr, TBP_Shadow);

		//Generate the shadow map for each mesh in the scene
		shadowCommandBuffer->beginEncoding(shadowRenderTarget);
		shadowCommandBuffer->clearTarget();
		shadowCommandBuffer->setDepthEnabled(true);
		shadowCommandBuffer->bindPipelineObject(*shadowPipelineObject);
		shadowCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });

		for (uint32_t i = 0; i < scene.numLights; ++i){
			shadowCommandBuffer->updateBufferData(*shadowInfoBuffer, &scene.shadowTransforms[i]);
			shadowCommandBuffer->bindShaderResourceBuffer(*shadowInfoBuffer, ShaderStage::Geometry, BBP_ShadowData);

			auto lightIndex = LightNumAlignment{ i * 6 };
			shadowCommandBuffer->updateBufferData(*lightIndexBuffer, &lightIndex);
			shadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Geometry, BBP_CurrentFaceIndex);

			shadowCommandBuffer->updateBufferData(*shadowLightPosBuffer, &scene.currentShadowDepth.depths[i]);
			shadowCommandBuffer->bindShaderResourceBuffer(*shadowLightPosBuffer, ShaderStage::Pixel, BBP_CurrentDepthData);

			for (auto& mesh : scene.meshes){
				mesh->draw(*shadowCommandBuffer, shadowPipelineObject->getVertexLayout());

				shadowCommandBuffer->drawIndexed(mesh->getIndexCount());
			}
		}

		//End encoding in order items need to be generated
		shadowCommandBuffer->endEncoding();
		meshCommandBuffer->endEncoding();
	}
}