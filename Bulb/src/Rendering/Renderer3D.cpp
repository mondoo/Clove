#include "Bulb/Rendering/Renderer3D.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"
#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"
#include "Bulb/Rendering/Renderables/Sprite.hpp"
#include "Bulb/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/CommandBuffer.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/PipelineObject.hpp>
#include <Clove/Graphics/RenderTarget.hpp>
#include <Clove/Graphics/Texture.hpp>
#include <Clove/Graphics/Surface.hpp>
#include <Clove/Platform/Window.hpp>

using namespace clv;
using namespace clv::gfx;

extern "C" const char	default3d_vs[];
extern "C" const size_t default3d_vsLength;
extern "C" const char	default3d_ps[];
extern "C" const size_t default3d_psLength;
extern "C" const char   skeletal3d_vs[];
extern "C" const size_t skeletal3d_vsLength;

extern "C" const char	default2d_vs[];
extern "C" const size_t default2d_vsLength;
extern "C" const char	default2d_ps[];
extern "C" const size_t default2d_psLength;

extern "C" const char	font_vs[];
extern "C" const size_t font_vsLength;
extern "C" const char	font_ps[];
extern "C" const size_t font_psLength;

extern "C" const char	genshadowmap_vs[];
extern "C" const size_t genshadowmap_vsLength;
extern "C" const char	genshadowmap_gs[];
extern "C" const size_t genshadowmap_gsLength;
extern "C" const char	genshadowmap_ps[];
extern "C" const size_t genshadowmap_psLength;

extern "C" const char	gencubeshadowmap_vs[];
extern "C" const size_t gencubeshadowmap_vsLength;
extern "C" const char	gencubeshadowmap_gs[];
extern "C" const size_t gencubeshadowmap_gsLength;
extern "C" const char	gencubeshadowmap_ps[];
extern "C" const size_t gencubeshadowmap_psLength;

namespace blb::rnd {
	Renderer3D::SceneData::SceneData() = default;

	Renderer3D::SceneData::~SceneData() = default;

	Renderer3D::Renderer3D(plt::Window& window) {
		GraphicsFactory& factory = *window.getGraphicsFactory();

		//Mesh
		meshCommandBuffer = factory.createCommandBuffer();

        auto meshVS        = factory.createShader({ ShaderStage::Vertex }, default3d_vs, default3d_vsLength);
        auto meshPS        = factory.createShader({ ShaderStage::Pixel }, default3d_ps, default3d_psLength);
        auto animMeshVS    = factory.createShader({ ShaderStage::Vertex }, skeletal3d_vs, skeletal3d_vsLength);

        meshPipelineObject = factory.createPipelineObject();
		meshPipelineObject->setVertexShader(*meshVS);
		meshPipelineObject->setPixelShader(*meshPS);

		animatedMeshPipelineObject = factory.createPipelineObject();
        animatedMeshPipelineObject->setVertexShader(*animMeshVS);
        animatedMeshPipelineObject->setPixelShader(*meshPS);

		//UI
		uiCommandBuffer = factory.createCommandBuffer();

		auto widgetVS = factory.createShader({ ShaderStage::Vertex }, default2d_vs, default2d_vsLength);
		auto widgetPS = factory.createShader({ ShaderStage::Pixel }, default2d_ps, default2d_psLength);
		widgetPipelineObject = factory.createPipelineObject();
		widgetPipelineObject->setVertexShader(*widgetVS);
		widgetPipelineObject->setPixelShader(*widgetPS);

		auto textVS = factory.createShader({ ShaderStage::Vertex }, font_vs, font_vsLength);
		auto textPS = factory.createShader({ ShaderStage::Pixel }, font_ps, font_psLength);
		textPipelineObject = factory.createPipelineObject();
		textPipelineObject->setVertexShader(*textVS);
		textPipelineObject->setPixelShader(*textPS);

		const std::vector<uint32_t> indices{
			0, 1, 2,
			0, 2, 3
		};
		BufferDescriptor ibDesc{};
		ibDesc.elementSize = sizeof(uint32_t);
		ibDesc.bufferSize = indices.size() * sizeof(uint32_t);
		ibDesc.bufferType = BufferType::IndexBuffer;
		ibDesc.bufferUsage = BufferUsage::Default;
		uiIndexBuffer = factory.createBuffer(ibDesc, indices.data());

		uiVbLayout.add(VertexElementType::position2D).add(VertexElementType::texture2D);

		BufferDescriptor vbDesc{};
		vbDesc.elementSize = uiVbLayout.size();
		//vbDesc.bufferSize	= ;
		vbDesc.bufferType = BufferType::VertexBuffer;
		vbDesc.bufferUsage = BufferUsage::Default;

		{ //Widgets
			//From top left
			VertexBufferData bufferData{ uiVbLayout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 0.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f, -1.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, -1.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 0.0f }, mth::vec2f{ 1.0f, 0.0f });

			vbDesc.bufferSize = bufferData.sizeBytes();
			widgetVB = factory.createBuffer(vbDesc, bufferData.data());
		}

		{ //Text
			//From bottom left
			VertexBufferData bufferData{ uiVbLayout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 0.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 1.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 1.0f }, mth::vec2f{ 0.0f, 0.0f });

			vbDesc.bufferSize = bufferData.sizeBytes();
			textVB = factory.createBuffer(vbDesc, bufferData.data());
		}

		//Directional shadow map
		gfx::TextureDescriptor dsDesc{};
		dsDesc.style		= TextureStyle::Default;
		dsDesc.usage		= TextureUsage::RenderTarget_Depth;
		dsDesc.dimensions	= { shadowMapSize, shadowMapSize };
		dsDesc.arraySize	= MAX_LIGHTS;

		directionalShadowMapTexture		= factory.createTexture(dsDesc, nullptr, 4);
		directionalShadowRenderTarget	= factory.createRenderTarget(nullptr, directionalShadowMapTexture.get());
		directionalShadowCommandBuffer	= factory.createCommandBuffer();

		auto dirShadowVS = factory.createShader({ ShaderStage::Vertex }, genshadowmap_vs, genshadowmap_vsLength);
		auto dirShadowGS = factory.createShader({ ShaderStage::Geometry }, genshadowmap_gs, genshadowmap_gsLength);
		auto dirShadowPS = factory.createShader({ ShaderStage::Pixel }, genshadowmap_ps, genshadowmap_psLength);
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

		auto pointShadowVS = factory.createShader({ ShaderStage::Vertex }, gencubeshadowmap_vs, gencubeshadowmap_vsLength);
		auto pointShadowGS = factory.createShader({ ShaderStage::Geometry }, gencubeshadowmap_gs, gencubeshadowmap_gsLength);
		auto pointShadowPS = factory.createShader({ ShaderStage::Pixel }, gencubeshadowmap_ps, gencubeshadowmap_psLength);
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

		bufferDesc.bufferSize = sizeof(DirectionalShadowTransform);
		directionalShadowTransformBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(DirectionalShadowTransformArray);
		directionalShadowTransformArrayBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(PointShadowTransform);
		pointShadowTransformBuffer = factory.createBuffer(bufferDesc, nullptr);

		bufferDesc.bufferSize = sizeof(NumberAlignment);
		lightIndexBuffer = factory.createBuffer(bufferDesc, nullptr);
	}

	void Renderer3D::begin() {
		CLV_PROFILE_FUNCTION();

		//Clear all render targets
		for(auto& camera : scene.cameras) {
			camera.target->clear();
		}

		scene.meshes.clear();
        scene.animatedMeshes.clear();
		scene.numDirectionalLights = 0;
		scene.numPointLights = 0;
		scene.cameras.clear();

		scene.textToRender.clear();
		scene.widgetsToRender.clear();
	}

	void Renderer3D::submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) {
		scene.meshes.push_back(mesh);
	}

	void Renderer3D::submitAnimatedMesh(const std::shared_ptr<rnd::Mesh>& mesh) {
        scene.animatedMeshes.push_back(mesh);
    }

	void Renderer3D::submitLight(const DirectionalLight& light) {
		const uint32_t lightIndex = scene.numDirectionalLights++;

		scene.lightDataArray.directionalLights[lightIndex] = light.data;
		scene.directionalShadowTransformArray[lightIndex] = light.shadowTransform;
	}

	void Renderer3D::submitLight(const PointLight& light) {
		const uint32_t lightIndex = scene.numPointLights++;

		scene.lightDataArray.pointLights[lightIndex] = light.data;
		scene.pointShadowTransformArray[lightIndex] = light.shadowTransforms;
	}

	void Renderer3D::submitCamera(const ComposedCameraData& camera) {
		scene.cameras.push_back(camera);
	}

	void Renderer3D::submitWidget(const std::shared_ptr<Sprite>& widget) {
		scene.widgetsToRender.push_back(widget);
	}

	void Renderer3D::submitText(const std::shared_ptr<Sprite>& text) {
		scene.textToRender.push_back(text);
	}

	void Renderer3D::end() {
		CLV_PROFILE_FUNCTION();

		//Draw all meshes in the scene
		meshCommandBuffer->setDepthEnabled(true);
		meshCommandBuffer->bindTexture(directionalShadowMapTexture.get(), TBP_DirectionalShadow);
		meshCommandBuffer->bindTexture(pointShadowMapTexture.get(), TBP_PointShadow);

		meshCommandBuffer->updateBufferData(*lightArrayBuffer, &scene.lightDataArray);
		meshCommandBuffer->bindShaderResourceBuffer(*lightArrayBuffer, ShaderStage::Pixel, BBP_LightData);
		//TODO: Remove duplicated updateBufferData
		pointShadowCommandBuffer->updateBufferData(*lightArrayBuffer, &scene.lightDataArray);
		pointShadowCommandBuffer->bindShaderResourceBuffer(*lightArrayBuffer, ShaderStage::Pixel, BBP_LightData);

		auto numLights = LightCount{ scene.numDirectionalLights, scene.numPointLights };
		meshCommandBuffer->updateBufferData(*lightNumBuffer, &numLights);
		meshCommandBuffer->bindShaderResourceBuffer(*lightNumBuffer, ShaderStage::Vertex, BBP_CurrentLights);
		meshCommandBuffer->bindShaderResourceBuffer(*lightNumBuffer, ShaderStage::Pixel, BBP_CurrentLights);

		meshCommandBuffer->updateBufferData(*directionalShadowTransformArrayBuffer, &scene.directionalShadowTransformArray);
		meshCommandBuffer->bindShaderResourceBuffer(*directionalShadowTransformArrayBuffer, ShaderStage::Vertex, BBP_AllDirectionalTransform);

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

			meshCommandBuffer->bindPipelineObject(*meshPipelineObject);
            for(auto& mesh : scene.meshes) { //TODO: We should submit the models instead of the underlying meshes
                mesh->draw(*meshCommandBuffer, meshPipelineObject->getVertexLayout());
            }

			//TODO: All meshes should be drawn like this with a identity matrix palette
            meshCommandBuffer->bindPipelineObject(*animatedMeshPipelineObject);
            for(auto& mesh : scene.animatedMeshes) {
                mesh->draw(*meshCommandBuffer, animatedMeshPipelineObject->getVertexLayout());
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
			directionalShadowCommandBuffer->updateBufferData(*directionalShadowTransformBuffer, &scene.directionalShadowTransformArray[i]);
			directionalShadowCommandBuffer->bindShaderResourceBuffer(*directionalShadowTransformBuffer, ShaderStage::Vertex, BBP_DirectionalShadowTransform);

			auto lightIndex = NumberAlignment{ i };
			directionalShadowCommandBuffer->updateBufferData(*lightIndexBuffer, &lightIndex);
			directionalShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Geometry, BBP_CurrentFaceIndex);

			for(auto& mesh : scene.meshes) {
                mesh->draw(*directionalShadowCommandBuffer, directionalShadowPipelineObject->getVertexLayout());
            }
            /*for(auto& mesh : scene.animatedMeshes) {
                mesh->draw(*directionalShadowCommandBuffer, directionalShadowPipelineObject->getVertexLayout());
            }*/
		}

		//Generate the point shadow map for each mesh in the scene
		pointShadowCommandBuffer->beginEncoding(pointShadowRenderTarget);
		pointShadowCommandBuffer->clearTarget();
		pointShadowCommandBuffer->setDepthEnabled(true);
		pointShadowCommandBuffer->bindPipelineObject(*pointShadowPipelineObject);
		pointShadowCommandBuffer->setViewport({ 0, 0, shadowMapSize, shadowMapSize });

		for(int32_t i = 0; i < scene.numPointLights; ++i) {
			pointShadowCommandBuffer->updateBufferData(*pointShadowTransformBuffer, &scene.pointShadowTransformArray[i]);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*pointShadowTransformBuffer, ShaderStage::Geometry, BBP_PointShadowTransform);

			auto lightIndex = NumberAlignment{ i * 6 };
			pointShadowCommandBuffer->updateBufferData(*lightIndexBuffer, &lightIndex);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Geometry, BBP_CurrentFaceIndex);
			pointShadowCommandBuffer->bindShaderResourceBuffer(*lightIndexBuffer, ShaderStage::Pixel, BBP_CurrentFaceIndex);

			for(auto& mesh : scene.meshes) {
				mesh->draw(*pointShadowCommandBuffer, pointShadowPipelineObject->getVertexLayout());
			}
            /*for(auto& mesh : scene.animatedMeshes) {
                mesh->draw(*directionalShadowCommandBuffer, directionalShadowPipelineObject->getVertexLayout());
            }*/
		}

		//Render each UI element
		const uint32_t vbStride = static_cast<uint32_t>(uiVbLayout.size());

		//TODO: We don't want the UI on every render target
		for(auto& camera : scene.cameras) {
			uiCommandBuffer->beginEncoding(camera.target);

			uiCommandBuffer->setViewport(camera.viewport);
			uiCommandBuffer->setDepthEnabled(false);

			uiCommandBuffer->bindPipelineObject(*widgetPipelineObject);

			//Widgets
			for(auto& sprite : scene.widgetsToRender) {
				sprite->getMaterialInstance().bind(*uiCommandBuffer);

				uiCommandBuffer->bindVertexBuffer(*widgetVB, vbStride);
				uiCommandBuffer->bindIndexBuffer(*uiIndexBuffer);

				uiCommandBuffer->drawIndexed(6u); //TODO: Remove hard coded value
			}

			uiCommandBuffer->bindPipelineObject(*textPipelineObject);

			//Text
			for(auto& text : scene.textToRender) {
				text->getMaterialInstance().bind(*uiCommandBuffer);

				uiCommandBuffer->bindVertexBuffer(*textVB, vbStride);
				uiCommandBuffer->bindIndexBuffer(*uiIndexBuffer);

				uiCommandBuffer->drawIndexed(6u); //TODO: Remove hard coded value
			}
		}
		

		//End encoding in order items need to be generated
		directionalShadowCommandBuffer->endEncoding();
		pointShadowCommandBuffer->endEncoding();
		meshCommandBuffer->endEncoding();
		uiCommandBuffer->endEncoding();
	}
}