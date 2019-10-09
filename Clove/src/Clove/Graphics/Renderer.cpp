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
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::gfx{
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;

	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO;
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO;

	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO;
	PointLightShaderData Renderer::currentLightInfo;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowShaderData>> Renderer::shadowDataSBO;
	PointShadowShaderData Renderer::currentShadowInfo;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowDepthData>> Renderer::shadowDepthData;
	PointShadowDepthData Renderer::currentShadowDepth;

	std::shared_ptr<gfx::Shader> Renderer::cubeShadowMapShader;
	std::shared_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::shadowModelData;
	const math::Matrix4f Renderer::shadowProj = math::createPerspectiveMatrix(math::asRadians(90.0f), aspect, nearDist, farDist);

	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	CameraRenderData Renderer::cameraSubmissionData;

	std::shared_ptr<RenderTarget> Renderer::shadowMapRenderTarget;
	std::shared_ptr<Texture> Renderer::shadowMapTexture;
	
	std::shared_ptr<RenderTarget> Renderer::customRenderTarget;

	void Renderer::initialise(){
		materialSBO = BindableFactory::createShaderBufferObject<MaterialData>(ShaderType::Pixel, BBP_MaterialData);

		viewDataSBO = BindableFactory::createShaderBufferObject<ViewData>(ShaderType::Vertex, BBP_CameraMatrices);
		viewPosSBO = BindableFactory::createShaderBufferObject<ViewPos>(ShaderType::Pixel, BBP_ViewData);
		
		lightDataSBO = BindableFactory::createShaderBufferObject<PointLightShaderData>(ShaderType::Pixel, BBP_PointLightData);
		shadowDataSBO = BindableFactory::createShaderBufferObject<PointShadowShaderData>(ShaderType::Geometry, BBP_ShadowData);

		cubeShadowMapShader = gfx::BindableFactory::createShader(gfx::ShaderStyle::CubeShadowMap);
		shadowModelData = BindableFactory::createShaderBufferObject<VertexData>(ShaderType::Vertex, BBP_ModelData);
		shadowDepthData = BindableFactory::createShaderBufferObject<PointShadowDepthData>(ShaderType::Pixel, BBP_CubeDepthData);

		materialSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();
		shadowDataSBO->bind();
		shadowDepthData->bind();

		materialSBO->update({ 32.0f });

		shadowMapTexture = BindableFactory::createTexture(shadowMapSize, shadowMapSize, TextureUsage::RenderTarget_Depth, TBP_Shadow, TextureStyle::Cubemap);
		shadowMapRenderTarget = RenderTarget::createRenderTarget(nullptr, shadowMapTexture.get());

		RenderCommand::setBlendState(true);
	}

	void Renderer::beginScene(){
		//currentLightInfo.numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);
		shadowDataSBO->update(currentShadowInfo);
		shadowDepthData->update(currentShadowDepth);

		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		//TODO: anyway to use above lambda?
		const auto drawShadow = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			cubeShadowMapShader->bind(); //Will DX like this?
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		RenderCommand::setDepthBuffer(true);

		//Calculate shadow map
		/*
		NOTE:
		It looks like I might need to generate a cube map texture for every light and then pass those to the lighting shader
			-This seems pretty nuts - I should get this working for one light first and then expand
				-I was thinking about all lights writing to the same cube map but that'll produce hard shadows and might overwrite some
				 I think I'll literally need to render the scene for every light
			-Unless there is some way to bind an array of cube maps into the shader. I'll do the mad expensive way first then investigate
		*/
		RenderCommand::setViewPortSize(shadowMapSize, shadowMapSize);
		RenderCommand::setRenderTarget(*shadowMapRenderTarget);
		std::for_each(meshesToRender.begin(), meshesToRender.end(), drawShadow);
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
		//const int32 lightIndex = currentLightInfo.numLights++;

		currentLightInfo.intensity/*.intensities[lightIndex]*/ = data.intensity;

		currentShadowInfo.shadowTransforms/*.shadowTransforms[lightIndex]*/ = data.shadowTransforms;

		currentShadowDepth.farPlane = farDist;
		currentShadowDepth.lightPos = data.intensity.position;
	}
}