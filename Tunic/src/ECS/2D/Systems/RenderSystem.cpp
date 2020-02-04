#include "Tunic/ECS/2D/Systems/RenderSystem.hpp"

#include "Tunic/Application.hpp"
#include "Tunic/ECS/Core/Manager.hpp"
#include "Tunic/ECS/2D/Components/TransformComponent.hpp"
#include "Tunic/ECS/2D/Components/SpriteComponent.hpp"
#include "Tunic/ECS/UI/Components/TransformComponent.hpp"
#include "Tunic/ECS/UI/Components/TextComponent.hpp"
#include "Tunic/ECS/UI/Components/WidgetComponent.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Tunic/Rendering/Renderables/Mesh.hpp"
#include "Tunic/Rendering/Renderables/Sprite.hpp"
#include "Clove/Graphics/Core/CommandBuffer.hpp"
#include "Clove/Graphics/Core/GraphicsFactory.hpp"
#include "Clove/Graphics/Core/Shader.hpp"
#include "Clove/Graphics/Core/PipelineObject.hpp"
#include "Clove/Platform/Core/Window.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::ecs::_2D{
	RenderSystem::RenderSystem(){
		const std::vector<uint32_t> indices = {
			0, 1, 2,
			0, 2, 3
		};

		VertexLayout layout;
		layout.add(VertexElementType::position2D).add(VertexElementType::texture2D);

		//Sprite mesh
		{
			//From the center
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ -0.5f,  0.5f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ -0.5f, -0.5f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{  0.5f, -0.5f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{  0.5f,  0.5f }, mth::vec2f{ 1.0f, 0.0f });

			auto spriteMaterial = std::make_shared<rnd::Material>();
			sceneData.spriteMesh = std::make_shared<rnd::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Widget mesh
		{
			//From top left
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  0.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f, -1.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, -1.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  0.0f }, mth::vec2f{ 1.0f, 1.0f });

			auto spriteMaterial = std::make_shared<rnd::Material>();
			sceneData.widgetMesh = std::make_shared<rnd::Mesh>(bufferData, indices, spriteMaterial->createInstance());
		}

		//Font mesh
		{
			//From bottom left
			VertexBufferData bufferData{ layout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  0.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f,  1.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f,  1.0f }, mth::vec2f{ 0.0f, 0.0f });

			auto characterMaterial = std::make_shared<rnd::Material>();
			sceneData.characterMesh = std::make_shared<rnd::Mesh>(bufferData, indices, characterMaterial->createInstance());
		}

		GraphicsFactory& graphicsFactory = Application::get().getGraphicsFactory();

		auto spriteVS = Application::get().getGraphicsFactory().createShader({ ShaderStage::Vertex }, "res/Shaders/2D-vs.hlsl");
		auto spritePS = Application::get().getGraphicsFactory().createShader({ ShaderStage::Pixel }, "res/Shaders/2D-ps.hlsl");
		sceneData.spritePipelineObject = graphicsFactory.createPipelineObject();
		sceneData.spritePipelineObject->setVertexShader(*spriteVS);
		sceneData.spritePipelineObject->setPixelShader(*spritePS);

		auto fontVS = Application::get().getGraphicsFactory().createShader({ ShaderStage::Vertex }, "res/Shaders/Font-vs.hlsl");
		auto fontPS = Application::get().getGraphicsFactory().createShader({ ShaderStage::Pixel }, "res/Shaders/Font-ps.hlsl");
		sceneData.charPipelineObject = graphicsFactory.createPipelineObject();
		sceneData.charPipelineObject->setVertexShader(*fontVS);
		sceneData.charPipelineObject->setPixelShader(*fontPS);
		
		commandBuffer = graphicsFactory.createCommandBuffer(Application::get().getMainWindow().getSurface());
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::preUpdate(){
		commandBuffer->beginEncoding();

		sceneData.spritesToRender.clear();
		sceneData.widgetsToRender.clear();
		sceneData.charactersToRender.clear();
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
				renderable->sprite->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * modelData, ShaderStage::Vertex);

				sceneData.spritesToRender.push_back(renderable->sprite);
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

				renderable->sprite->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * modelData, ShaderStage::Vertex);

				sceneData.widgetsToRender.push_back(renderable->sprite);
			}
		}

		//Characters
		{
			CLV_PROFILE_SCOPE("Preparing characters");

			GraphicsFactory& graphicsFactory = Application::get().getGraphicsFactory();

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

				const rnd::Text& text = fontComp->text;
				mth::vec2f cursorPos = transform->getPosition();
				cursorPos.x -= (screenHalfSize.x - offset.x);
				cursorPos.y += (screenHalfSize.y + offset.y);

				for(size_t i = 0; i < text.getTextLength(); ++i){
					rnd::Glyph glyph = text.getBufferForCharAt(i);

					//For spaces we just skip and proceed
					if(glyph.buffer){
						const float width = glyph.size.x;
						const float height = glyph.size.y;

						const float xpos = cursorPos.x + glyph.bearing.x;
						const float ypos = cursorPos.y - (height - glyph.bearing.y);

						const uint8_t textureArraySize = 1;
						const TextureDescriptor descriptor = {
							TextureStyle::Default,
							TextureUsage::Font,
							TextureFilter::Nearest,
							{ width, height },
							textureArraySize
						};

						auto texture = graphicsFactory.createTexture(descriptor, glyph.buffer, 1);

						mth::mat4f model = mth::mat4f(1.0f);
						model = mth::translate(mth::mat4f(1.0f), { xpos, ypos, 0.0f });
						model *= mth::scale(mth::mat4f(1.0f), { width, height, 0.0f });

						auto character = std::make_shared<rnd::Sprite>(texture);
						character->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * model, ShaderStage::Vertex);

						sceneData.charactersToRender.push_back(character);
					}

					cursorPos.x += glyph.advance.x;
				}
			}
		}
	}

	void RenderSystem::postUpdate(){
		CLV_PROFILE_FUNCTION();

		const mth::vec2i screenSize = Application::get().getMainWindow().getSize();
		
		commandBuffer->setViewport({ 0,0, screenSize.x, screenSize.y });
		commandBuffer->setDepthEnabled(false);

		commandBuffer->bindPipelineObject(*sceneData.spritePipelineObject);
		//Sprites
		{
			const auto draw = [this](const std::shared_ptr<rnd::Sprite>& sprite){
				sprite->getMaterialInstance().bind(commandBuffer);

				const auto vertexLayout = sceneData.spritePipelineObject->getVertexLayout();

				auto vb = sceneData.spriteMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = sceneData.spriteMesh->getIndexBuffer();

				commandBuffer->bindVertexBuffer(*vb, static_cast<uint32_t>(vertexLayout.size()));
				commandBuffer->bindIndexBuffer(*ib);

				commandBuffer->drawIndexed(sceneData.spriteMesh->getIndexCount());
			};

			std::for_each(sceneData.spritesToRender.begin(), sceneData.spritesToRender.end(), draw);
		}

		//Widgets
		{
			const auto draw = [this](const std::shared_ptr<rnd::Sprite>& sprite){
				sprite->getMaterialInstance().bind(commandBuffer);

				const auto vertexLayout = sceneData.spritePipelineObject->getVertexLayout();

				auto vb = sceneData.widgetMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = sceneData.widgetMesh->getIndexBuffer();

				commandBuffer->bindVertexBuffer(*vb, static_cast<uint32_t>(vertexLayout.size()));
				commandBuffer->bindIndexBuffer(*ib);

				commandBuffer->drawIndexed(sceneData.widgetMesh->getIndexCount());
			};

			std::for_each(sceneData.widgetsToRender.begin(), sceneData.widgetsToRender.end(), draw);
		}

		commandBuffer->bindPipelineObject(*sceneData.charPipelineObject);
		//Characters
		{
			const auto draw = [this](const std::shared_ptr<rnd::Sprite>& character){
				character->getMaterialInstance().bind(commandBuffer);

				const auto vertexLayout = sceneData.charPipelineObject->getVertexLayout();

				auto vb = sceneData.characterMesh->getVertexBufferForLayout(vertexLayout);
				auto ib = sceneData.characterMesh->getIndexBuffer();

				commandBuffer->bindVertexBuffer(*vb, static_cast<uint32_t>(vertexLayout.size()));
				commandBuffer->bindIndexBuffer(*ib);

				commandBuffer->drawIndexed(sceneData.characterMesh->getIndexCount());
			};

			std::for_each(sceneData.charactersToRender.begin(), sceneData.charactersToRender.end(), draw);
		}

		commandBuffer->endEncoding();
	}
}