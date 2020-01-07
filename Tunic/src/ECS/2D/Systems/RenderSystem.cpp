#include "Tunic/ECS/2D/Systems/RenderSystem.hpp"

#include "Tunic/Application.hpp"
#include "Tunic/ECS/Core/Manager.hpp"
#include "Tunic/ECS/2D/Components/TransformComponent.hpp"
#include "Tunic/ECS/2D/Components/SpriteComponent.hpp"
#include "Tunic/ECS/UI/Components/TransformComponent.hpp"
#include "Tunic/ECS/UI/Components/TextComponent.hpp"
#include "Tunic/ECS/UI/Components/WidgetComponent.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/GraphicsGlobal.hpp"
#include "Clove/Graphics/Core/Renderables/Mesh.hpp"
#include "Clove/Graphics/Core/Renderables/Sprite.hpp"
#include "Clove/Graphics/Core/Shader.hpp"
#include "Clove/Graphics/Core/PipelineObject.hpp"
#include "Clove/Platform/Core/Window.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::ecs::_2D{
	struct SceneData{
		std::shared_ptr<gfx::Mesh> spriteMesh;
		std::shared_ptr<gfx::Mesh> widgetMesh;
		std::shared_ptr<gfx::Mesh> characterMesh;

		std::vector<std::shared_ptr<Sprite>> spritesToRender;
		std::vector<std::shared_ptr<Sprite>> widgetsToRender;
		std::vector<std::shared_ptr<Sprite>> charactersToRender;

		std::shared_ptr<PipelineObject> spritePipelineObject;
		std::shared_ptr<PipelineObject> charPipelineObject;
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
			bufferData.emplaceBack(mth::vec2f{ -0.5f, -0.5f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{  0.5f, -0.5f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ -0.5f,  0.5f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{  0.5f,  0.5f }, mth::vec2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<gfx::Material>();
			currentSceneData->spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Widget mesh
		{
			//From top left
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, -1.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, -1.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  0.0f }, mth::vec2f{ 1.0f, 1.0f });

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

		currentSceneData->spritePipelineObject = global::graphicsFactory->createPipelineObject(global::graphicsFactory->createShader({ ShaderStyle::Unlit_2D }));
		currentSceneData->charPipelineObject = global::graphicsFactory->createPipelineObject(global::graphicsFactory->createShader({ ShaderStyle::Font }));
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem(){
		CLV_LOG_TRACE("Shutting down renderer");
		delete currentSceneData;
	}

	void RenderSystem::preUpdate(){
		CLV_PROFILE_FUNCTION();

		//Empty for now
	}

	void RenderSystem::update(utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize();
		const mth::vec2f screenHalfSize{ static_cast<float>(screenSize.x) / 2.0f, static_cast<float>(screenSize.y) / 2.0f };
		const mth::mat4f projection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);

		//Srpites
		{
			CLV_PROFILE_SCOPE("Preparing sprites");

			auto componentTuples = manager->getComponentSets<TransformComponent, SpriteComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				SpriteComponent* renderable = std::get<SpriteComponent*>(tuple);

				const mth::mat4f modelData = transform->getWorldTransformMatrix();
				renderable->sprite->setModelData(projection * modelData);

				currentSceneData->spritesToRender.push_back(renderable->sprite);
			}
		}

		//Widgets
		{
			CLV_PROFILE_SCOPE("Preparing widgets");

			auto componentTuples = manager->getComponentSets<ui::TransformComponent, ui::WidgetComponent>();
			for(auto& tuple : componentTuples){
				ui::TransformComponent* transform = std::get<ui::TransformComponent*>(tuple);
				ui::WidgetComponent* renderable = std::get<ui::WidgetComponent*>(tuple);

				const mth::vec2f widgetScale = transform->getScale();
				const mth::vec2f scaledScreenSize = { (screenHalfSize.x / widgetScale.x), (screenHalfSize.y / widgetScale.y) };

				mth::vec2f offset{};
				const mth::vec2f anchor = transform->getAnchor();

				if(ui::TransformComponent* parent = transform->getParent()){
					const mth::vec2f parentScale = parent->getScale();
					offset.x = (anchor.x * parentScale.x) / widgetScale.x;
					offset.y = (anchor.y * parentScale.y) / widgetScale.y;
				} else{
					offset.x = (anchor.x * screenSize.x) / widgetScale.x;
					offset.y = (anchor.y * screenSize.y) / widgetScale.y;
				}

				const mth::mat4f modelData = mth::translate(transform->getWorldTransformMatrix(), mth::vec3f{ -scaledScreenSize.x + offset.x, scaledScreenSize.y - offset.y, 0.0f });

				renderable->sprite->setModelData(projection * modelData);

				currentSceneData->widgetsToRender.push_back(renderable->sprite);
			}
		}

		//Characters
		{
			CLV_PROFILE_SCOPE("Preparing characters");

			auto componentTuples = manager->getComponentSets<ui::TransformComponent, ui::TextComponent>();
			for(auto& tuple : componentTuples){
				ui::TransformComponent* transform = std::get<ui::TransformComponent*>(tuple);
				ui::TextComponent* fontComp = std::get<ui::TextComponent*>(tuple);

				mth::vec2f offset{};
				const mth::vec2f anchor = transform->getAnchor();

				if(ui::TransformComponent* parent = transform->getParent()){
					const mth::vec2f parentScale = parent->getScale();
					offset.x = anchor.x * parentScale.x;
					offset.y = anchor.y * parentScale.y;
				} else{
					offset.x = anchor.x * screenSize.x;
					offset.y = anchor.y * screenSize.y;
				}

				const clv::ui::Text& text = fontComp->text;
				mth::vec2f cursorPos = transform->getPosition();
				cursorPos.x -= (screenHalfSize.x - offset.x);
				cursorPos.y += (screenHalfSize.y + offset.y);

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
							gfx::TextureFilter::Nearest,
							{ width, height },
							textureArraySize
						};

						auto texture = gfx::global::graphicsFactory->createTexture(descriptor, glyph.buffer, 1);

						mth::mat4f model = mth::mat4f(1.0f);
						model = mth::translate(mth::mat4f(1.0f), { xpos, ypos, 0.0f });
						model *= mth::scale(mth::mat4f(1.0f), { width, height, 0.0f });

						auto character = std::make_shared<gfx::Sprite>(texture);
						character->setModelData(projection * model);

						currentSceneData->charactersToRender.push_back(character);
					}

					cursorPos.x += glyph.advance.x;
				}
			}
		}
	}

	void RenderSystem::postUpdate(){
		CLV_PROFILE_FUNCTION();

		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize();

		global::graphicsDevice->setViewport({ 0, 0, screenSize.x, screenSize.y });
		global::graphicsDevice->setDepthBuffer(false);
		global::graphicsDevice->setRenderTargetToDefault();

		//Sprites / Widgets
		global::graphicsDevice->bindPipelineObject(*currentSceneData->spritePipelineObject);
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

				global::graphicsDevice->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				global::graphicsDevice->bindIndexBuffer(*ib);

				global::graphicsDevice->drawIndexed(currentSceneData->spriteMesh->getIndexCount());
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

				global::graphicsDevice->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				global::graphicsDevice->bindIndexBuffer(*ib);

				global::graphicsDevice->drawIndexed(currentSceneData->widgetMesh->getIndexCount());
			};

			std::for_each(currentSceneData->widgetsToRender.begin(), currentSceneData->widgetsToRender.end(), draw);
			currentSceneData->widgetsToRender.clear();
		}

		//Characters
		global::graphicsDevice->bindPipelineObject(*currentSceneData->charPipelineObject);
		{
			const auto draw = [](const std::shared_ptr<Sprite>& character){
				auto& charMat = currentSceneData->characterMesh->getMaterialInstance();
				charMat.setAlbedoTexture(character->getTexture());
				charMat.setData(BBP_2DData, character->getModelData(), ShaderType::Vertex);
				charMat.bind();

				const auto vertexLayout = currentSceneData->charPipelineObject->getVertexLayout();

				auto vb = currentSceneData->characterMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = currentSceneData->characterMesh->getIndexBuffer();

				global::graphicsDevice->bindVertexBuffer(*vb, static_cast<uint32>(vertexLayout.size()));
				global::graphicsDevice->bindIndexBuffer(*ib);

				global::graphicsDevice->drawIndexed(currentSceneData->characterMesh->getIndexCount());
			};

			std::for_each(currentSceneData->charactersToRender.begin(), currentSceneData->charactersToRender.end(), draw);
			currentSceneData->charactersToRender.clear();
		}
	}
}