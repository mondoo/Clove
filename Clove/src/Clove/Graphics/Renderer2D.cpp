#include "clvpch.hpp"
#include "Renderer2D.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/VertexLayout.hpp"

namespace clv::gfx {
	std::shared_ptr<gfx::Mesh> Renderer2D::renderMesh;
	std::shared_ptr<Material> Renderer2D::meshMaterial;

	std::queue<std::shared_ptr<Sprite>> Renderer2D::renderQueue;

	void Renderer2D::initialise() {
		//VB
		VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(VertexElementType::texture2D);
		VertexBufferData bufferData(std::move(layout));
		bufferData.emplaceBack(math::Vector2f{ -1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ 1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ -1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
		bufferData.emplaceBack(math::Vector2f{ 1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

		//IB
		std::vector<uint32> indices = {
			1, 3, 0,
			3, 2, 0
		};

		meshMaterial = std::make_shared<gfx::Material>(gfx::ShaderStyle::_2D);
		renderMesh = std::make_shared<gfx::Mesh>(bufferData, indices, meshMaterial->createInstance());
	}

	void Renderer2D::beginScene() {
		//TODO
	}

	void Renderer2D::endScene() {
		RenderCommand::setDepthBuffer(false);

		renderMesh->bind();

		while(!renderQueue.empty()){
			auto& sprite = renderQueue.front();

			meshMaterial->setAlbedoTexture(sprite->getTexture());
			meshMaterial->setData(BBP_2DData, sprite->getModelData(), ShaderType::Vertex);
			meshMaterial->bind();
			RenderCommand::drawIndexed(renderMesh->getIndexCount());

			renderQueue.pop();
		}
	}

	void Renderer2D::submitSprite(const std::shared_ptr<Sprite> &sprite){
		renderQueue.push(sprite);
	}
}