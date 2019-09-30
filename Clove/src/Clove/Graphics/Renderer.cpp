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

namespace clv::gfx{
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;

	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO;
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO;

	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO;
	PointLightShaderData Renderer::currentLightInfo;

	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	CameraRenderData Renderer::cameraSubmissionData;

	std::shared_ptr<RenderTarget> Renderer::renderTarget;

	void Renderer::initialise(){
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);

		viewDataSBO = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderType::Vertex, gfx::BBP_CameraMatrices);
		viewPosSBO = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderType::Pixel, gfx::BBP_ViewData);
		
		lightDataSBO = gfx::BindableFactory::createShaderBufferObject<PointLightShaderData>(gfx::ShaderType::Pixel, gfx::BBP_PointLightData);

		materialSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();

		materialSBO->update({ 32.0f });

		RenderCommand::setBlendState(true);
	}

	void Renderer::beginScene(){
		currentLightInfo.numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);

		if(renderTarget){
			RenderCommand::setRenderTarget(*renderTarget);
			RenderCommand::clear();
		}

		RenderCommand::setDepthBuffer(true);

		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);

		if(renderTarget){
			RenderCommand::resetRenderTarget();
		}

		meshesToRender.clear();
	}

	void Renderer::setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget){
		renderTarget = inRenderTarget;
	}

	void Renderer::removeRenderTarget(){
		renderTarget.reset();
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
		currentLightInfo.lights[currentLightInfo.numLights++] = data;
	}
}