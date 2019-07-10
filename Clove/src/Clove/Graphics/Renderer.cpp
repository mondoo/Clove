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

	void Renderer::initialise(){
		vertCB = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialCB = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);

		mData.sininess = 32.0f;
		materialCB->update(mData);
	}

	void Renderer::beginScene(){
		RenderCommand::clear();

		vertCB->bind();
		materialCB->bind();
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

	}
}