#include "TextSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/2D/Components/TransformComponent.hpp"
#include "Core/ECS/UI/Components/TextComponent.hpp"
#include "Core/Graphics/Renderer2D.hpp"
#include "Core/Graphics/Renderables/Sprite.hpp"
#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/RenderCommand.hpp"
#include "Core/UI/Font.hpp"
#include "Core/UI/Text.hpp"

namespace clv::ecs::ui{
	TextSystem::TextSystem() = default;

	TextSystem::~TextSystem() = default;

	void TextSystem::update(utl::DeltaTime deltaTime){
		auto componentTuples = manager->getComponentSets<_2D::TransformComponent, TextComponent>();

		for(auto& tuple : componentTuples){
			_2D::TransformComponent* transform = std::get<_2D::TransformComponent*>(tuple);
			TextComponent* fontComp = std::get<TextComponent*>(tuple);

			const clv::ui::Text& text = fontComp->text;
			math::Vector2f cursorPos = transform->getPosition();

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

					math::Matrix4f model = math::Matrix4f(1.0f);
					model = math::translate(math::Matrix4f(1.0f), { xpos, ypos, 0.0f });
					model *= math::scale(math::Matrix4f(1.0f), { width, height, 0.0f });

					auto character = std::make_shared<gfx::Sprite>(texture);
					character->setModelData(gfx::Renderer2D::getSpriteProjection() * model);

					gfx::Renderer2D::submitCharacter(character);
				}
				
				cursorPos.x += glyph.advance.x;
			}
		}
	}
}