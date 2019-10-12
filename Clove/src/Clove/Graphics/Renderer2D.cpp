#include "Renderer2D.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"

namespace clv::gfx{
	std::shared_ptr<gfx::Mesh> Renderer2D::spriteMesh;
	std::shared_ptr<gfx::Mesh> Renderer2D::characterMesh;

	std::vector<std::shared_ptr<Sprite>> Renderer2D::spritesToRender;
	std::vector<std::shared_ptr<Sprite>> Renderer2D::charactersToRender;

	math::Matrix4f Renderer2D::projection;

	void Renderer2D::initialise(){
		const std::vector<uint32> indices = {
			1, 3, 0,
			3, 2, 0
		};

		//Sprite mesh
		{
			VertexLayout layout;
			layout.add(gfx::VertexElementType::position2D).add(VertexElementType::texture2D);
			VertexBufferData bufferData(std::move(layout));
			bufferData.emplaceBack(math::Vector2f{ -1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{  1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ -1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{  1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<gfx::Material>(gfx::ShaderStyle::_2D);
			spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Font mesh
		{
			VertexLayout layout;
			layout.add(gfx::VertexElementType::position2D).add(VertexElementType::texture2D);
			VertexBufferData bufferData(std::move(layout));
			bufferData.emplaceBack(math::Vector2f{ 0,  0 }, math::Vector2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{ 1,  0 }, math::Vector2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{ 0,  1 }, math::Vector2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ 1,  1 }, math::Vector2f{ 1.0f, 0.0f });

			auto characterMaterial = std::make_shared<gfx::Material>(gfx::ShaderStyle::Font);
			characterMesh = std::make_shared<gfx::Mesh>(bufferData, indices, characterMaterial->createInstance());
		}

		//Projection
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		projection = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	void Renderer2D::beginScene(){
		//TODO
	}

	void Renderer2D::endScene(){
		RenderCommand::setDepthBuffer(false);

		//Sprites
		{
			const auto draw = [](const std::shared_ptr<Sprite>& sprite){
				auto& renderMeshMaterial = spriteMesh->getMaterialInstance();
				renderMeshMaterial.setAlbedoTexture(sprite->getTexture());
				renderMeshMaterial.setData(BBP_2DData, sprite->getModelData(), ShaderType::Vertex);
				spriteMesh->bind();

				RenderCommand::drawIndexed(spriteMesh->getIndexCount());
			};

			std::for_each(spritesToRender.begin(), spritesToRender.end(), draw);

			spritesToRender.clear();
		}

		//Characters
		{
			const auto draw = [](const std::shared_ptr<Sprite>& character){
				auto& charMat = characterMesh->getMaterialInstance();
				charMat.setAlbedoTexture(character->getTexture());
				charMat.setData(BBP_2DData, character->getModelData(), ShaderType::Vertex);
				characterMesh->bind();

				RenderCommand::drawIndexed(characterMesh->getIndexCount());
			};

			std::for_each(charactersToRender.begin(), charactersToRender.end(), draw);

			charactersToRender.clear();
		}
	}

	void Renderer2D::submitSprite(const std::shared_ptr<Sprite> &sprite){
		spritesToRender.push_back(sprite);
	}

	void Renderer2D::submitCharacter(const std::shared_ptr<Sprite>& character){
		charactersToRender.push_back(character);
	}

	const math::Matrix4f &Renderer2D::getSpriteProjection(){
		return projection;
	}
}