#include "Tunic/Rendering/Renderer.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"
#include "Tunic/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/Core/CommandBuffer.hpp>
#include <Clove/Graphics/Core/GraphicsFactory.hpp>
#include <Clove/Graphics/Core/PipelineObject.hpp>
#include <Clove/Graphics/Core/RenderTarget.hpp>
#include <Clove/Graphics/Core/Resources/Texture.hpp>
#include <Clove/Graphics/Core/Surface.hpp>
#include <Clove/Platform/Core/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd {
	Renderer::SceneData::SceneData() = default;

	Renderer::SceneData::~SceneData() = default;

	Renderer::Renderer(plt::Window& window) {

		GraphicsFactory& factory = window.getGraphicsFactory();

		//Mesh command buffer
		meshCommandBuffer = factory.createCommandBuffer();

		auto meshVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/Default3D-vs.hlsl");
		auto meshPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/Default3D-ps.hlsl");
		meshPipelineObject = factory.createPipelineObject();
		meshPipelineObject->setVertexShader(*meshVS);
		meshPipelineObject->setPixelShader(*meshPS);

		//Directional shadow map
		gfx::TextureDescriptor dsDesc{};
		dsDesc.style		= TextureStyle::Default;
		dsDesc.usage		= TextureUsage::RenderTarget_Depth;
		dsDesc.dimensions	= { shadowMapSize, shadowMapSize };
		dsDesc.arraySize	= MAX_LIGHTS;

		directionalShadowMapTexture		= factory.createTexture(dsDesc, nullptr, 4);
		directionalShadowRenderTarget	= factory.createRenderTarget(nullptr, directionalShadowMapTexture.get());
		directionalShadowCommandBuffer	= factory.createCommandBuffer();

		auto dirShadowVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/GenShadowMap-vs.hlsl");
		auto dirShadowGS = factory.createShader({ ShaderStage::Geometry }, "res/Shaders/GenShadowMap-gs.hlsl");
		auto dirShadowPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/GenShadowMap-ps.hlsl");
		directionalShadowPipelineObject = factory.createPipelineObject();
		directionalShadowPipelineObject->setVertexShader(*dirShadowVS);
		directionalShadowPipelineObject->setGeometryShader(*dirShadowGS);
		directionalShadowPipelineObject->setPixelShader(*dirShadowPS);

		//Point shadow map
		gfx::TextureDescriptor psDesc{};
		psDesc.style		= TextureStyle::Cubemap;
		psDesc.usage		= TextureUsage::RenderTarget_Depth;
		psDesc.dimensions	= { shadowMapSize, shadowMapSize };
		psDesc.arraySize	= MAX_LIGHTS;

		pointShadowMapTexture		= factory.createTexture(psDesc, nullptr, 4);
		pointShadowRenderTarget		= factory.createRenderTarget(nullptr, pointShadowMapTexture.get());
		pointShadowCommandBuffer	= factory.createCommandBuffer();

		auto pointShadowVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/GenCubeShadowMap-vs.hlsl");
		auto pointShadowGS = factory.createShader({ ShaderStage::Geometry }, "res/Shaders/GenCubeShadowMap-gs.hlsl");
		auto pointShadowPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/GenCubeShadowMap-ps.hlsl");
		pointShadowPipelineObject = factory.createPipelineObject();
		pointShadowPipelineObject->setVertexShader(*pointShadowVS);
		pointShadowPipelineObject->setGeometryShader(*pointShadowGS);
		pointShadowPipelineObject->setPixelShader(*pointShadowPS);

		//Buffers
		gfx::BufferDescriptor bufferDesc{};
		bufferDesc.elementSize	= 0;
		bufferDesc.bufferType	= clv::gfx::BufferType::ShaderResourceBuffer;
		bufferDesc.bufferUsage	= clv::gfx::BufferUsage::Dynamic;

		bufferDesc.bufferSize = sizeof(ViewData);
		viewBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(ViewPos);
		viewPosition = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(LightDataArray);
		lightArrayBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(LightCount);
		lightNumBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(PointShadowTransform);
		pointShadowTransformBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(NumberAlignment);
		lightIndexBuffer = factory.createBuffer(bufferDesc, nullptr);
	}

	void Renderer::begin() {
		CLV_PROFILE_FUNCTION();

		scene.meshes.clear();
		scene.numDirectionalLights = 0;
		scene.numPointLights = 0;
		scene.cameras.clear();
	}

	void Renderer::submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) {
		scene.meshes.push_back(mesh);
	}

	void Renderer::submitLight(const DirectionalLight& light) {
		const uint32_t lightIndex = scene.numDirectionalLights++;

		scene.lightDataArray.directionalLights[lightIndex] = light.data;
		scene.directionalShadowTransformArray[lightIndex] = light.shadowTransform;
	}

	void Renderer::submitLight(const PointLight& light) {
		const uint32_t lightIndex = scene.numPointLights++;

		scene.lightDataArray.pointLights[lightIndex] = light.data;
		scene.pointShadowTransformArray[lightIndex] = light.shadowTransforms;
	}

	void Renderer::submitCamera(const ComposedCameraData& camera) {
		scene.cameras.push_back(camera);
	}

	void Renderer::end() {
		CLV_PROFILE_FUNCTION();

		//Draw all meshes in the scene
		meshCommandBuffer->setDepthEnabled(true);
		meshCommandBuffer->bindPipelineObject(*meshPipelineObject);
		meshCommandBuffer->bindTexture(directionalShadowMapTexture.get(), TBP_DirectionalShadow);
		meshCommandBuffer->bindTexture(pointShadowMapTexture.get(), TBP_PointShadow);

		meshCommandBuffer->updateBufferData(*lightArrayBuffer, &scene.lightDataArray);
		meshCommandBuffer->bindShaderResourceBuffer(*lightArrayBuffer, ShaderStage::Pixel, BBP_PointLightData);
		//TODO: Remove duplocated updateBufferData
		pointShadowCommandBuffer->updateBufferData(*lightArrayBuffer, &scene.lightDataArray);
		pointShadowCommandBuffer->bindShaderResourceBuffer(*lightArrayBuffer, ShaderStage::Pixel, BBP_PointLightData);

		auto numLights = LightCount{ scene.numDirectionalLights, scene.numPointLights };
		meshCommandBuffer->updateBufferData(*lightNumBuffer, &numLights);
		meshCommandBuffer->bindShaderResourceBuffer(*lightNumBuffer, ShaderStage::Pixel, BBP_CurrentLights);

		for(auto& camera : scene.cameras) {
			meshCommandBuffer->beginEncoding(camera.target);

			meshCommandBuffer->setViewport(camera.viewport);

			const CameraRenderData& camRenderData = camera.bufferData;

			auto viewData = ViewData{ camRenderData.lookAt, camRenderData.projection };
			meshCommandBuffer->updateBufferData(*viewBuffer, &viewData);
			meshCommandBuffer->bindShaderResourceBuffer(*viewBuffer, ShaderStage::Vertex, BBP_CameraMatrices);

			auto viewPos = ViewPos{ camRenderData.position };
			meshCommandBuffer->updateBufferData(*viewPosition, &viewPos);
			meshCommandBuffer->bindShaderResourceBuffer(*viewPosition, ShaderStage::Pixel, BBP_ViewData);

			for(auto& mesh : scene.meshes) {
				mesh->draw(*meshCommandBuffer, meshPipelineObject->getVertexLayout());
			}
		}

		meshCommandBuffer->bindTexture(nullptr, TBP_DirectionalShadow);
		meshCommandBuffer->bindTexture(nullptr, TBP_PointShadow);

		//Generate the directional shadow map for each mesh in the scene
		directionalShadowCommandBuffer->beginEncoding(directionalShadowRenderTarget);
		directionalShadowCommandBuffer->clearTarget();
		directionalShadowCommandBuffer->setDepthEnabled(true);
		directionalShadowCommandBuffer->bindPipelineObject(*directionalShadowPipelineObject);
		directionalShadowCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });

		for(int32_t i = 0; i < scene.numDirectionalLights; ++i) {
			auto lightIndex = NumberAlignment{ i };
			directionalShadowCommandBuffer->updateBufferData(*lightIndexBuffer, &lightIndex);
			directionalShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Geometry, BBP_CurrentFaceIndex);

			for(auto& mesh : scene.meshes) {
				mesh->draw(*directionalShadowCommandBuffer, directionalShadowPipelineObject->getVertexLayout());

				directionalShadowCommandBuffer->drawIndexed(mesh->getIndexCount());
			}
		}

		//Generate the point shadow map for each mesh in the scene
		pointShadowCommandBuffer->beginEncoding(pointShadowRenderTarget);
		pointShadowCommandBuffer->clearTarget();
		pointShadowCommandBuffer->setDepthEnabled(true);
		pointShadowCommandBuffer->bindPipelineObject(*pointShadowPipelineObject);
		pointShadowCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });

		for(int32_t i = 0; i < scene.numPointLights; ++i) {
			pointShadowCommandBuffer->updateBufferData(*pointShadowTransformBuffer, &scene.pointShadowTransformArray[i]);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*pointShadowTransformBuffer, ShaderStage::Geometry, BBP_ShadowData);

			auto lightIndex = NumberAlignment{ i * 6 };
			pointShadowCommandBuffer->updateBufferData(*lightIndexBuffer, &lightIndex);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Geometry, BBP_CurrentFaceIndex);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Pixel, BBP_CurrentFaceIndex);

			for(auto& mesh : scene.meshes) {
				mesh->draw(*pointShadowCommandBuffer, pointShadowPipelineObject->getVertexLayout());

				pointShadowCommandBuffer->drawIndexed(mesh->getIndexCount());
			}
		}

		//End encoding in order items need to be generated
		directionalShadowCommandBuffer->endEncoding();
		pointShadowCommandBuffer->endEncoding();
		meshCommandBuffer->endEncoding();
	}
}