#include "TextSystem.hpp"

#include "Clove/Graphics/Renderer2D.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/UI/Font.hpp"
#include "Clove/UI/Text.hpp"

namespace clv::ecs::ui{
	TextSystem::TextSystem() = default;

	TextSystem::~TextSystem() = default;

	void TextSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			d2::TransformComponent* transform = std::get<d2::TransformComponent*>(componentTuple);
			TextComponent* fontComp = std::get<TextComponent*>(componentTuple);

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

					auto texture = gfx::BindableFactory::createTexture(glyph.buffer, width, height, gfx::TBP_Albedo, gfx::TextureUsage::Font);

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