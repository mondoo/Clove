#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	std::unique_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::vertSBO;
	std::unique_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;
	std::unique_ptr<gfx::ShaderBufferObject<SpriteShaderData>> Renderer::spriteSBO;

	void Renderer::initialise(){
		vertSBO = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);
		spriteSBO = gfx::BindableFactory::createShaderBufferObject<SpriteShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);

		materialSBO->update({ 32.0f });
	}

	void Renderer::beginScene(){
		RenderCommand::clear();

		vertSBO->bind();
		materialSBO->bind();
		spriteSBO->bind();
	}

	void Renderer::endScene(){
		
	}

	void Renderer::submitMesh(SubmitData data){
		vertSBO->update({ data.modelData, math::transpose(math::inverse(data.modelData)) });

		for(const auto& bindable : data.bindables){
			bindable->bind();
		}

		RenderCommand::drawIndexed(data.indexCount);
	}

	void Renderer::submitSprite(SubmitData data){
		spriteSBO->update({ data.modelData });

		for(const auto& bindable : data.bindables){
			bindable->bind();
		}

		RenderCommand::drawIndexed(data.indexCount);
	}
}