#include "Renderer2D.hpp"

#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/Resources/Buffer.hpp"
#include "Core/Graphics/PipelineObject.hpp"
#include "Core/Graphics/Renderables/Sprite.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/VertexLayout.hpp"
#include "Core/Platform/Application.hpp"
#include "Core/Platform/Window.hpp"

namespace clv::gfx{
	struct SceneData2D{
		std::shared_ptr<gfx::Mesh> spriteMesh;
		std::shared_ptr<gfx::Mesh> characterMesh;

		std::vector<std::shared_ptr<Sprite>> spritesToRender;
		std::vector<std::shared_ptr<Sprite>> charactersToRender;

		//std::shared_ptr<Buffer> vertexBuffer;
		//std::shared_ptr<Buffer> indexBuffer;
		std::shared_ptr<PipelineObject> spritePipelineObject;
		std::shared_ptr<PipelineObject> charPipelineObject;

		math::Matrix4f projection; //Effectively the 'camera'
	} *currentSceneData2D;

	void Renderer2D::initialise(){
		CLV_LOG_TRACE("Initialising renderer");
		currentSceneData2D = new SceneData2D();

		const std::vector<uint32> indices = {
			1, 3, 0,
			3, 2, 0
		};

		VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(VertexElementType::texture2D);
		
		//Sprite mesh
		{
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(math::Vector2f{ -1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{  1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ -1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{  1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<gfx::Material>(/*gfx::ShaderStyle::Unlit_2D*/);
			currentSceneData2D->spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Font mesh
		{
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(math::Vector2f{ 0,  0 }, math::Vector2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{ 1,  0 }, math::Vector2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{ 0,  1 }, math::Vector2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ 1,  1 }, math::Vector2f{ 1.0f, 0.0f });

			auto characterMaterial = std::make_shared<gfx::Material>(/*gfx::ShaderStyle::Font*/);
			currentSceneData2D->characterMesh = std::make_shared<gfx::Mesh>(bufferData, indices, characterMaterial->createInstance());
		}

		currentSceneData2D->spritePipelineObject = RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::Unlit_2D }));
		currentSceneData2D->charPipelineObject = RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::Font }));

		//Projection
		const float halfWidth = static_cast<float>(plt::Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(plt::Application::get().getWindow().getHeight()) / 2;

		currentSceneData2D->projection = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	void Renderer2D::shutDown(){
		CLV_LOG_TRACE("Shutting down renderer");
		delete currentSceneData2D;
	}

	void Renderer2D::beginScene(){
		//TODO
	}

	void Renderer2D::endScene(){
		RenderCommand::setDepthBuffer(false);

		RenderCommand::bindPipelineObject(*currentSceneData2D->spritePipelineObject);

		//Sprites
		{
			const auto draw = [](const std::shared_ptr<Sprite>& sprite){
				auto& renderMeshMaterial = currentSceneData2D->spriteMesh->getMaterialInstance();
				renderMeshMaterial.setAlbedoTexture(sprite->getTexture());
				renderMeshMaterial.setData(BBP_2DData, sprite->getModelData(), ShaderType::Vertex);
				//Temp
				auto vb = currentSceneData2D->spriteMesh->generateVertexBuffer(currentSceneData2D->spritePipelineObject->getVertexLayout());
				auto ib = currentSceneData2D->spriteMesh->getIndexBuffer();
				//~
				renderMeshMaterial.bind();
				RenderCommand::bindVertexBuffer(*vb);
				RenderCommand::bindIndexBuffer(*ib);

				RenderCommand::drawIndexed(currentSceneData2D->spriteMesh->getIndexCount());
			};

			std::for_each(currentSceneData2D->spritesToRender.begin(), currentSceneData2D->spritesToRender.end(), draw);

			currentSceneData2D->spritesToRender.clear();
		}

		RenderCommand::bindPipelineObject(*currentSceneData2D->charPipelineObject);

		//Characters
		{
			const auto draw = [](const std::shared_ptr<Sprite>& character){
				auto& charMat = currentSceneData2D->characterMesh->getMaterialInstance();
				charMat.setAlbedoTexture(character->getTexture());
				charMat.setData(BBP_2DData, character->getModelData(), ShaderType::Vertex);
				//Temp
				auto vb = currentSceneData2D->characterMesh->generateVertexBuffer(currentSceneData2D->charPipelineObject->getVertexLayout());
				auto ib = currentSceneData2D->characterMesh->getIndexBuffer();
				//~
				charMat.bind();
				RenderCommand::bindVertexBuffer(*vb);
				RenderCommand::bindIndexBuffer(*ib);

				RenderCommand::drawIndexed(currentSceneData2D->characterMesh->getIndexCount());
			};

			std::for_each(currentSceneData2D->charactersToRender.begin(), currentSceneData2D->charactersToRender.end(), draw);

			currentSceneData2D->charactersToRender.clear();
		}
	}

	void Renderer2D::submitSprite(const std::shared_ptr<Sprite> &sprite){
		currentSceneData2D->spritesToRender.push_back(sprite);
	}

	void Renderer2D::submitCharacter(const std::shared_ptr<Sprite>& character){
		currentSceneData2D->charactersToRender.push_back(character);
	}

	const math::Matrix4f &Renderer2D::getSpriteProjection(){
		return currentSceneData2D->projection;
	}
}