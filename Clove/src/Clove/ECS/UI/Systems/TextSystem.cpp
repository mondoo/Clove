#include "clvpch.hpp"
#include "TextSystem.hpp"

//Should all be temp
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer2D.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

//Temp
#include <fstream>
#include <strstream>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "Clove/UI/Font.hpp"
#include "Clove/UI/Text.hpp"

namespace clv::ecs::ui{
	TextSystem::TextSystem() = default;

	TextSystem::~TextSystem() = default;

	void TextSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			d2::TransformComponent* transform = std::get<d2::TransformComponent*>(componentTuple);
			TextComponent* fontComp = std::get<TextComponent*>(componentTuple);

			//TODO: Using hard coded values for now - needs transform component

			///*const*/ math::Matrix4f model = math::Matrix4f(1.0f); /*transform->getWorldTransformMatrix();*/
			
			const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
			const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;
			const auto spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

			clv::ui::Text text(clv::ui::Font("res/Fonts/Roboto/Roboto-Black.ttf"));
			text.setSize(72);
			text.setText("Hello, World!");

			float cursorPos = -550.0f;
			for(size_t i = 0; i < text.getTextLength(); ++i){
				clv::ui::Glyph glyph = text.getBufferForCharAt(i);

				//For spaces we just skip and proceed
				if(glyph.buffer){
					const float width = glyph.size.x;
					const float height = glyph.size.y;

					const float x = glyph.bearing.x;
					const float y = (height - glyph.bearing.y);

					const float xpos = cursorPos + x;
					const float ypos = -y;

					auto texture = gfx::BindableFactory::createTexture(glyph.buffer, width, height, gfx::TBP_Albedo, gfx::TextureUsage::Font);

					math::Matrix4f model = math::Matrix4f(1.0f);
					model = math::translate(math::Matrix4f(1.0f), { xpos, ypos, 0.0f });
					model *= math::scale(math::Matrix4f(1.0f), { width, height, 0.0f });

					auto character = std::make_shared<gfx::Sprite>(texture);
					character->setModelData(spriteProj * model);

					gfx::Renderer2D::submitCharacter(character);
				}
				
				cursorPos += glyph.advance.x;
			}
		}
	}
}