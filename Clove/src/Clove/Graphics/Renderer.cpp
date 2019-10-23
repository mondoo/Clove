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
	

	

	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO;
	PointLightShaderData Renderer::currentLightInfo;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowShaderData>> Renderer::shadowDataSBO;
	PointShadowShaderData Renderer::currentShadowInfo;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowDepthData>> Renderer::shadowDepthData;
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowData>> Renderer::currentDepthData;
	PointShadowDepthData Renderer::currentShadowDepth;
	std::shared_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::shadowModelData;
	std::shared_ptr<gfx::Shader> Renderer::cubeShadowMapShader;
	std::shared_ptr<gfx::ShaderBufferObject<LightNumAlignment>> Renderer::lightNumSBO;
	std::shared_ptr<gfx::ShaderBufferObject<LightNumAlignment>> Renderer::lightNumSBOGS;
	uint32 Renderer::numLights;

	std::array<std::array<math::Matrix4f, 6>, MAX_LIGHTS> Renderer::shadowTransforms = {};

	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	std::shared_ptr<RenderTarget> Renderer::shadowMapRenderTarget;
	std::shared_ptr<Texture> Renderer::shadowMapTexture;
	
	std::shared_ptr<RenderTarget> Renderer::customRenderTarget;

	void Renderer::initialise(){
		materialSBO = BindableFactory::createShaderBufferObject<MaterialData>(ShaderType::Pixel, BBP_MaterialData);

		viewDataSBO = BindableFactory::createShaderBufferObject<ViewData>(ShaderType::Vertex, BBP_CameraMatrices);
		viewPosSBO = BindableFactory::createShaderBufferObject<ViewPos>(ShaderType::Pixel, BBP_ViewData);
		
		lightDataSBO = BindableFactory::createShaderBufferObject<PointLightShaderData>(ShaderType::Pixel, BBP_PointLightData);
		shadowDataSBO = BindableFactory::createShaderBufferObject<PointShadowShaderData>(ShaderType::Geometry, BBP_ShadowData);
		shadowDepthData = BindableFactory::createShaderBufferObject<PointShadowDepthData>(ShaderType::Pixel, BBP_CubeDepthData);
		currentDepthData = BindableFactory::createShaderBufferObject<PointShadowData>(ShaderType::Pixel, BBP_CurrentDepthData);
		shadowModelData = BindableFactory::createShaderBufferObject<VertexData>(ShaderType::Vertex, BBP_ModelData);
		cubeShadowMapShader = BindableFactory::createShader(gfx::ShaderStyle::CubeShadowMap);
		lightNumSBO = BindableFactory::createShaderBufferObject<LightNumAlignment>(ShaderType::Pixel, BBP_CurrentLights);
		lightNumSBOGS = BindableFactory::createShaderBufferObject<LightNumAlignment>(ShaderType::Geometry, BBP_CurrentLightIndex);

		materialSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();
		shadowDataSBO->bind();
		shadowDepthData->bind();
		lightNumSBO->bind();
		lightNumSBOGS->bind();

		materialSBO->update({ 32.0f });

		shadowMapTexture = BindableFactory::createTexture(TBP_Shadow, { TextureStyle::Cubemap, TextureUsage::RenderTarget_Depth, { shadowMapSize, shadowMapSize }, MAX_LIGHTS });
		shadowMapRenderTarget = RenderTarget::createRenderTarget(nullptr, shadowMapTexture.get());

		RenderCommand::setBlendState(true);
	}

	void Renderer::shutDown(){
		CLV_LOG_TRACE("Shutting down renderer");

	}

	void Renderer::beginScene(){
		numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);
		shadowDepthData->update(currentShadowDepth);
		lightNumSBO->update({ numLights });

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
		for(uint32 i = 0; i < numLights; i++){ //TODO: Remove raw loop
			shadowDataSBO->bind();
			shadowDataSBO->update({ shadowTransforms[i] });
			currentDepthData->bind();
			currentDepthData->update({ currentShadowDepth.depths[i] });

			lightNumSBOGS->update({ i * 6 });

			RenderCommand::setViewPortSize(shadowMapSize, shadowMapSize);
			RenderCommand::setRenderTarget(*shadowMapRenderTarget); //TODO: How to set view level????
			std::for_each(meshesToRender.begin(), meshesToRender.end(), drawShadow);
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
		viewDataSBO->update({ data.lookAt, data.projection });
		viewPosSBO->update({ data.position });
	}

	void Renderer::submitPointLight(const PointLightData& data){
		const int32 lightIndex = numLights++;
		currentLightInfo.intensities[lightIndex] = data.intensity;

		shadowTransforms[lightIndex] = data.shadowTransforms;

		currentShadowDepth.depths[lightIndex].farPlane = data.farPlane;
		currentShadowDepth.depths[lightIndex].lightPos = data.intensity.position;
	}
}