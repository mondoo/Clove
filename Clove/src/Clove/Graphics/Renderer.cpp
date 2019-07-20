#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	std::shared_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::vertSBO;
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;
	std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> Renderer::spriteSBO;

	std::vector<SubmitData> Renderer::meshSubmissionData;
	std::vector<SubmitData> Renderer::spriteSubmissionData;

	void Renderer::initialise(){
		vertSBO = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);
		spriteSBO = gfx::BindableFactory::createShaderBufferObject<SpriteShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);

		vertSBO->bind();
		materialSBO->bind();
		spriteSBO->bind();

		materialSBO->update({ 32.0f });

		RenderCommand::setBlendState(true);
	}

	void Renderer::beginScene(){
		
	}

	void Renderer::endScene(){
		//Mesh
		RenderCommand::setDepthBuffer(true);
		for(auto& data : meshSubmissionData){
			vertSBO->update({ data.modelData, math::transpose(math::inverse(data.modelData)) });
			for(const auto& bindable : data.bindables){
				bindable->bind();
			}
			RenderCommand::drawIndexed(data.indexCount);
		}
		meshSubmissionData.clear();

		//Sprite
		RenderCommand::setDepthBuffer(false);
		for(auto& data : spriteSubmissionData){
			spriteSBO->update({ data.modelData });
			for(const auto& bindable : data.bindables){
				bindable->bind();
			}
			RenderCommand::drawIndexed(data.indexCount);
		}
		spriteSubmissionData.clear();
	}

	void Renderer::submitMesh(SubmitData data){
		meshSubmissionData.emplace_back(std::move(data));
	}

	void Renderer::submitSprite(SubmitData data){
		spriteSubmissionData.emplace_back(std::move(data));
	}
}