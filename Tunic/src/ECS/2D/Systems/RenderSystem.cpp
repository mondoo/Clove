#include "Tunic/ECS/2D/Systems/RenderSystem.hpp"

#include "Tunic/Application.hpp"
#include "Tunic/ECS/Core/World.hpp"
#include "Tunic/ECS/2D/Components/TransformComponent.hpp"
#include "Tunic/ECS/2D/Components/SpriteComponent.hpp"
#include "Tunic/ECS/UI/Components/TransformComponent.hpp"
#include "Tunic/ECS/UI/Components/TextComponent.hpp"
#include "Tunic/ECS/UI/Components/WidgetComponent.hpp"
#include "Tunic/Rendering/Renderables/Sprite.hpp"
#include "Tunic/Rendering/Renderer2D.hpp"

#include <Clove/Platform/Core/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace tnc::ecs::_2D{
	RenderSystem::RenderSystem(rnd::Renderer2D* renderer)
		: renderer(renderer){
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::preUpdate(World& world){
		//TODO: Clear camera render targets when there are 2D camera components
	}

	void RenderSystem::update(World& world, utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize();
		const mth::vec2f screenHalfSize{ static_cast<float>(screenSize.x) / 2.0f, static_cast<float>(screenSize.y) / 2.0f };
		const mth::mat4f projection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);

		//Srpites
		{
			CLV_PROFILE_SCOPE("Preparing sprites");

			auto componentTuples = world.getComponentSets<TransformComponent, SpriteComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				SpriteComponent* renderable = std::get<SpriteComponent*>(tuple);

				const mth::mat4f modelData = transform->getWorldTransformMatrix();
				renderable->sprite->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * modelData, ShaderStage::Vertex);

				renderer->sumbitSprite(renderable->sprite);
			}
		}

		//Widgets
		{
			CLV_PROFILE_SCOPE("Preparing widgets");

			auto componentTuples = world.getComponentSets<ui::TransformComponent, ui::WidgetComponent>();
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

				renderer->submitWidget(renderable->sprite);
			}
		}

		//Characters
		{
			CLV_PROFILE_SCOPE("Preparing characters");

			GraphicsFactory& graphicsFactory = Application::get().getGraphicsFactory();

			auto componentTuples = world.getComponentSets<ui::TransformComponent, ui::TextComponent>();
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

						renderer->submitText(character);
					}

					cursorPos.x += glyph.advance.x;
				}
			}
		}
	}
}