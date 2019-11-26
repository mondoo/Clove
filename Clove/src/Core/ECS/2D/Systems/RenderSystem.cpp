#include "RenderSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/2D/Components/TransformComponent.hpp"
#include "Core/ECS/2D/Components/SpriteComponent.hpp"
#include "Core/ECS/UI/Components/TransformComponent.hpp"
#include "Core/ECS/UI/Components/TextComponent.hpp"
#include "Core/ECS/UI/Components/WidgetComponent.hpp"
#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/Renderables/Sprite.hpp"
#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/PipelineObject.hpp"
#include "Core/Platform/Application.hpp"
#include "Core/Platform/Window.hpp"

using namespace clv::gfx;

namespace clv::ecs::_2D{
	struct SceneData{
		std::shared_ptr<gfx::Mesh> spriteMesh;
		std::shared_ptr<gfx::Mesh> widgetMesh;
		std::shared_ptr<gfx::Mesh> characterMesh;

		std::vector<std::shared_ptr<Sprite>> spritesToRender;
		std::vector<std::shared_ptr<Sprite>> widgetsToRender;
		std::vector<std::shared_ptr<Sprite>> charactersToRender;

		std::shared_ptr<PipelineObject> spritePipelineObject;
		std::shared_ptr<PipelineObject> charPipelineObject;

		mth::mat4f projection; //Effectively the 'camera'
		mth::vec2ui screenSize;
	} *currentSceneData;

	RenderSystem::RenderSystem(){
		CLV_LOG_TRACE("Initialising renderer");
		currentSceneData = new SceneData();

		const std::vector<uint32> indices = {
			1, 3, 0,
			3, 2, 0
		};

		VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(VertexElementType::texture2D);

		//Sprite mesh
		{
			//From the center
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ -1.0f, -1.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{  1.0f, -1.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ -1.0f,  1.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{  1.0f,  1.0f }, mth::vec2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<gfx::Material>();
			currentSceneData->spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Widget mesh
		{
			//From top left
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, -2.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 2.0f, -2.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 2.0f,  0.0f }, mth::vec2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<gfx::Material>();
			currentSceneData->widgetMesh = std::make_shared<gfx::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Font mesh
		{
			//From bottom left
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  0.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  1.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  1.0f }, mth::vec2f{ 1.0f, 0.0f });

			auto characterMaterial = std::make_shared<gfx::Material>();
			currentSceneData->characterMesh = std::make_shared<gfx::Mesh>(bufferData, indices, characterMaterial->createInstance());
		}

		currentSceneData->spritePipelineObject = RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::Unlit_2D }));
		currentSceneData->charPipelineObject = RenderCommand::createPipelineObject(RenderCommand::createShader({ ShaderStyle::Font }));

		//Projection
		plt::Application::get().getWindow().onWindowResize.bind(&RenderSystem::onWindowSizeChanged, this);

		currentSceneData->screenSize = { plt::Application::get().getWindow().getWidth(), plt::Application::get().getWindow().getHeight() };

		const float halfWidth = static_cast<float>(currentSceneData->screenSize.x) / 2;
		const float halfHeight = static_cast<float>(currentSceneData->screenSize.y) / 2;

		currentSceneData->projection = mth::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem(){
		CLV_LOG_TRACE("Shutting down renderer");
		delete currentSceneData;
	}

	void RenderSystem::preUpdate(){
		//Empty for now
	}

	void RenderSystem::update(utl::DeltaTime deltaTime){
		//Srpites
		{
			auto componentTuples = manager->getComponentSets<TransformComponent, SpriteComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				SpriteComponent* renderable = std::get<SpriteComponent*>(tuple);

				const mth::mat4f modelData = transform->getWorldTransformMatrix();
				renderable->sprite->setModelData(currentSceneData->projection * modelData);

				currentSceneData->spritesToRender.push_back(renderable->sprite);
			}
		}

		const mth::vec2f screenHalfSize{ static_cast<float>(currentSceneData->screenSize.x) / 2.0f, static_cast<float>(currentSceneData->screenSize.y) / 2.0f };

		//Widgets
		{
			auto componentTuples = manager->getComponentSets<ui::TransformComponent, ui::WidgetComponent>();
			for(auto& tuple : componentTuples){
				ui::TransformComponent* transform = std::get<ui::TransformComponent*>(tuple);
				ui::WidgetComponent* renderable = std::get<ui::WidgetComponent*>(tuple);

				const mth::vec2f widgetScale = transform->getScale();
				const mth::vec2f scaledScreenSize = { (screenHalfSize.x / widgetScale.x), (screenHalfSize.y / widgetScale.y) };

				const mth::mat4f modelData = mth::translate(transform->getWorldTransformMatrix(), mth::vec3f{ -scaledScreenSize.x, scaledScreenSize.y, 0.0f });

				renderable->sprite->setModelData(currentSceneData->projection * modelData);

				currentSceneData->widgetsToRender.push_back(renderable->sprite);
			}
		}

		//Characters
		{
			auto componentTuples = manager->getComponentSets<ui::TransformComponent, ui::TextComponent>();
			for(auto& tuple : componentTuples){
				ui::TransformComponent* transform = std::get<ui::TransformComponent*>(tuple);
				ui::TextComponent* fontComp = std::get<ui::TextComponent*>(tuple);

				const clv::ui::Text& text = fontComp->text;
				mth::vec2f cursorPos = transform->getPosition();
				cursorPos.x -= screenHalfSize.x;
				cursorPos.y += screenHalfSize.y;

				for(size_t i = 0; i < text.getTextLength(); ++i){
					clv::ui::Glyph glyph = text.getBufferForCharAt(i);

					//For spaces we just skip and proceed
					if(glyph.buffer){
						const float width = glyph.size.x;
						const float height = glyph.size.y;

						const float xpos = cursorPos.x + glyph.bearing.x;
						const float ypos = cursorPos.y - (height - glyph.bearing.y);

						const uint8 textureArraySize = 1;
						const gfx::TextureDescriptor descriptor = {
							gfx::TextureStyle::Default,
							gfx::TextureUsage::Font,
							{ width, height },
							textureArraySize
						};

						auto texture = gfx::RenderCommand::createTexture(descriptor, glyph.buffer, 1);

						mth::mat4f model = mth::mat4f(1.0f);
						model = mth::translate(mth::mat4f(1.0f), { xpos, ypos, 0.0f });
						model *= mth::scale(mth::mat4f(1.0f), { width, height, 0.0f });

						auto character = std::make_shared<gfx::Sprite>(texture);
						character->setModelData(currentSceneData->projection * model);

						currentSceneData->charactersToRender.push_back(character);
					}

					cursorPos.x += glyph.advance.x;
				}
			}
		}
	}

	void RenderSystem::postUpdate(){
		RenderCommand::setDepthBuffer(false);
		RenderCommand::resetRenderTargetToDefault();

		//Sprites / Widgets
		RenderCommand::bindPipelineObject(*currentSceneData->spritePipelineObject);
		{
			const auto draw = [](const std::shared_ptr<Sprite>& sprite){
				auto& renderMeshMaterial = currentSceneData->spriteMesh->getMaterialInstance();
				renderMeshMaterial.setAlbedoTexture(sprite->getTexture());
				renderMeshMaterial.setData(BBP_2DData, sprite->getModelData(), ShaderType::Vertex);
				renderMeshMaterial.setData(BBP_Colour, sprite->getColour(), ShaderType::Pixel);
				renderMeshMaterial.bind();

				const auto vertexLayout = currentSceneData->spritePipelineObject->getVertexLayout();

				auto vb = currentSceneData->spriteMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = currentSceneData->spriteMesh->getIndexBuffer();

				RenderCommand::bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				RenderCommand::bindIndexBuffer(*ib);

				RenderCommand::drawIndexed(currentSceneData->spriteMesh->getIndexCount());
			};

			std::for_each(currentSceneData->spritesToRender.begin(), currentSceneData->spritesToRender.end(), draw);
			currentSceneData->spritesToRender.clear();
		}

		//Widgets
		{
			const auto draw = [](const std::shared_ptr<Sprite>& sprite){
				auto& renderMeshMaterial = currentSceneData->widgetMesh->getMaterialInstance();
				renderMeshMaterial.setAlbedoTexture(sprite->getTexture());
				renderMeshMaterial.setData(BBP_2DData, sprite->getModelData(), ShaderType::Vertex);
				renderMeshMaterial.setData(BBP_Colour, sprite->getColour(), ShaderType::Pixel);
				renderMeshMaterial.bind();

				const auto vertexLayout = currentSceneData->spritePipelineObject->getVertexLayout();

				auto vb = currentSceneData->widgetMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = currentSceneData->widgetMesh->getIndexBuffer();

				RenderCommand::bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				RenderCommand::bindIndexBuffer(*ib);

				RenderCommand::drawIndexed(currentSceneData->widgetMesh->getIndexCount());
			};

			std::for_each(currentSceneData->widgetsToRender.begin(), currentSceneData->widgetsToRender.end(), draw);
			currentSceneData->widgetsToRender.clear();
		}

		//Characters
		RenderCommand::bindPipelineObject(*currentSceneData->charPipelineObject);
		{
			const auto draw = [](const std::shared_ptr<Sprite>& character){
				auto& charMat = currentSceneData->characterMesh->getMaterialInstance();
				charMat.setAlbedoTexture(character->getTexture());
				charMat.setData(BBP_2DData, character->getModelData(), ShaderType::Vertex);
				charMat.bind();

				const auto vertexLayout = currentSceneData->charPipelineObject->getVertexLayout();

				auto vb = currentSceneData->characterMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = currentSceneData->characterMesh->getIndexBuffer();

				RenderCommand::bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				RenderCommand::bindIndexBuffer(*ib);

				RenderCommand::drawIndexed(currentSceneData->characterMesh->getIndexCount());
			};

			std::for_each(currentSceneData->charactersToRender.begin(), currentSceneData->charactersToRender.end(), draw);
			currentSceneData->charactersToRender.clear();
		}
	}

	void RenderSystem::onWindowSizeChanged(const mth::vec2ui& size){
		const float halfWidth = static_cast<float>(size.x) / 2;
		const float halfHeight = static_cast<float>(size.y) / 2;

		currentSceneData->screenSize = size;
		currentSceneData->projection = mth::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}
}