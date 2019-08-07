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
#include "Clove/Graphics/Mesh.hpp"

namespace clv::gfx{
	void SpriteRenderData::bind() const{
		texture->bind();
	}

	//std::shared_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::vertSBO;
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;
	std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> Renderer::spriteSBO;

	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO;
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO;

	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO;
	PointLightShaderData Renderer::currentLightInfo;

	std::queue<std::shared_ptr<Mesh>> Renderer::meshRenderQueue;
	std::vector<SpriteRenderData> Renderer::spriteSubmissionData;
	CameraRenderData Renderer::cameraSubmissionData;

	std::shared_ptr<VertexBuffer> Renderer::spriteVBBuffer;
	std::shared_ptr<IndexBuffer> Renderer::spriteIBBuffer;
	std::shared_ptr<Shader> Renderer::spriteShader;
	math::Matrix4f Renderer::spriteProj = {};

	std::shared_ptr<RenderTarget> Renderer::renderTarget;

	void Renderer::initialise(){
		//vertSBO = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);
		spriteSBO = gfx::BindableFactory::createShaderBufferObject<SpriteShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);

		viewDataSBO = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderType::Vertex, gfx::BBP_CameraMatrices);
		viewPosSBO = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderType::Pixel, gfx::BBP_ViewData);
		
		lightDataSBO = gfx::BindableFactory::createShaderBufferObject<PointLightShaderData>(gfx::ShaderType::Pixel, gfx::BBP_PointLightData);

		//vertSBO->bind();
		materialSBO->bind();
		spriteSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();

		materialSBO->update({ 32.0f });

		RenderCommand::setBlendState(true);

		//QUAD STUFF
		//Shader
		spriteShader = gfx::BindableFactory::createShader(ShaderStyle::_2D);

		//VB
		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
		gfx::VertexBufferData bufferData(std::move(layout));
		bufferData.emplaceBack(math::Vector2f{-1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ 1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{-1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
		bufferData.emplaceBack(math::Vector2f{ 1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

		spriteVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *spriteShader);

		//IB
		std::vector<unsigned int> indices = {
			1, 3, 0,
			3, 2, 0
		};
		spriteIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

		//Proj
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	void Renderer::beginScene(){
		currentLightInfo.numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);

		if(renderTarget){
			RenderCommand::setRenderTarget(*renderTarget);
			RenderCommand::clear(); //make sure it's clean
		}

		//MESH
		RenderCommand::setDepthBuffer(true);

		while(!meshRenderQueue.empty()){
			auto& mesh = meshRenderQueue.front();
			mesh->bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
			meshRenderQueue.pop();
		}

		if(renderTarget){
			RenderCommand::resetRenderTarget();
		}

		//SPRITE
		RenderCommand::setDepthBuffer(false);

		spriteVBBuffer->bind();
		spriteIBBuffer->bind();
		spriteShader->bind();
		spriteSBO->bind();

		for(auto& data : spriteSubmissionData){
			spriteSBO->update({ spriteProj * data.modelData });
			data.bind();
			RenderCommand::drawIndexed(spriteIBBuffer->getIndexCount());
		}

		spriteSubmissionData.clear();
	}

	void Renderer::setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget){
		renderTarget = inRenderTarget;
	}

	void Renderer::removeRenderTarget(){
		renderTarget.reset();
		RenderCommand::resetRenderTarget();
	}

	void Renderer::submitMesh(const std::shared_ptr<Mesh>& data){
		meshRenderQueue.push(data);
	}

	void Renderer::submitSprite(const SpriteRenderData& data){
		spriteSubmissionData.emplace_back(data);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		viewDataSBO->update({ data.lookAt, data.projection });
		viewPosSBO->update({ data.position });
	}

	void Renderer::submitPointLight(const PointLightData& data){
		//lightDataSBO->update(data);
		currentLightInfo.lights[currentLightInfo.numLights++] = data;
	}
}