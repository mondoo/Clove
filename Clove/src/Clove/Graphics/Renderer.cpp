#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	std::unique_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::vertCB;
	VertexData Renderer::vData{};

	std::unique_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialCB;
	MaterialData Renderer::mData{};

	std::unique_ptr<gfx::ShaderBufferObject<SpriteShaderData>> Renderer::spriteSBO;
	SpriteShaderData Renderer::sData{};

	void Renderer::initialise(){
		vertCB = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialCB = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);
		spriteSBO = gfx::BindableFactory::createShaderBufferObject<SpriteShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);

		mData.sininess = 32.0f;
		materialCB->update(mData);
	}

	void Renderer::beginScene(){
		RenderCommand::clear();

		vertCB->bind();
		materialCB->bind();
		spriteSBO->bind();
	}

	void Renderer::endScene(){
		
	}

	void Renderer::submitMesh(SubmitData data){
		vData.model = data.modelData;
		vData.normalMatrix = math::transpose(math::inverse(data.modelData));
		vertCB->update(vData);

		for(const auto& bindable : data.bindables){
			bindable->bind();
		}

		RenderCommand::drawIndexed(data.indexCount);
	}

	void Renderer::submitSprite(SubmitData data){
		sData.modelProjection = data.modelData;
		spriteSBO->update(sData);

		for(const auto& bindable : data.bindables){
			bindable->bind();
		}

		RenderCommand::drawIndexed(data.indexCount);
	}
}