#include "clvpch.hpp"
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
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"

namespace clv::gfx{
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;

	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO;
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO;

	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO;
	PointLightShaderData Renderer::currentLightInfo;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowShaderData>> Renderer::shadowDataSBO;
	PointShadowShaderData Renderer::currentShadowInfo;

	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	CameraRenderData Renderer::cameraSubmissionData;

	std::shared_ptr<RenderTarget> Renderer::shadowMapRenderTarget;
	std::shared_ptr<Texture> Renderer::shadowMapTexture;
	
	std::shared_ptr<RenderTarget> Renderer::customRenderTarget;

	void Renderer::initialise(){
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);

		viewDataSBO = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderType::Vertex, gfx::BBP_CameraMatrices);
		viewPosSBO = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderType::Pixel, gfx::BBP_ViewData);
		
		lightDataSBO = gfx::BindableFactory::createShaderBufferObject<PointLightShaderData>(gfx::ShaderType::Pixel, gfx::BBP_PointLightData);
		shadowDataSBO = gfx::BindableFactory::createShaderBufferObject<PointShadowShaderData>(gfx::ShaderType::Pixel, gfx::BBP_ShadowData);

		materialSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();

		materialSBO->update({ 32.0f });

		shadowMapTexture = BindableFactory::createTexture(shadowMapSize, shadowMapSize, TextureUsage::RenderTarget_Depth, TBP_Shadow, TextureStyle::Cubemap);
		shadowMapRenderTarget = RenderTarget::createRenderTarget(nullptr, shadowMapTexture.get());

		RenderCommand::setBlendState(true);
	}

	void Renderer::beginScene(){
		currentLightInfo.numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);
		shadowDataSBO->update(currentShadowInfo);

		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		RenderCommand::setDepthBuffer(true);

		//Calculate shadow map
		RenderCommand::setViewPortSize(shadowMapSize, shadowMapSize);
		RenderCommand::setRenderTarget(*shadowMapRenderTarget);
		std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);
		RenderCommand::setViewPortSize(Application::get().getWindow().getWidth(), Application::get().getWindow().getHeight());

		shadowMapTexture->bind();

		//Render any other render targets
		if(customRenderTarget){
			RenderCommand::setRenderTarget(*customRenderTarget);
			std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);
		}

		RenderCommand::resetRenderTarget();

		//Render scene
		std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);

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
	}

	void Renderer::removeRenderTarget(){
		customRenderTarget.reset();
		RenderCommand::resetRenderTarget();
	}

	void Renderer::submitMesh(const std::shared_ptr<Mesh>& mesh){
		meshesToRender.push_back(mesh);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		viewDataSBO->update({ data.lookAt, data.projection });
		viewPosSBO->update({ data.position });
	}

	void Renderer::submitPointLight(const PointLightData& data){
		const int32 lightIndex = currentLightInfo.numLights++;
		currentLightInfo.intensities[lightIndex] = data.intensity;
		currentShadowInfo.shadowTransforms[lightIndex] = data.shadowTransforms;
	}
}