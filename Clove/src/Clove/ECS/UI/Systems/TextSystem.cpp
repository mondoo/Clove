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
#include <ft2build.h>
#include FT_FREETYPE_H

namespace clv::ecs::ui{
	TextSystem::TextSystem(){
		//Just getting something on the screen for now
		//sprite = std::make_shared<gfx::Sprite>("res/Textures/Zombie-32x32.png");

		if(FT_Init_FreeType(&ft)){
			CLV_ASSERT(false, "Could not load freetype");
		}

		if(FT_New_Face(ft, "res/Fonts/Roboto/Roboto-Black.ttf", 0, &face)){
			CLV_ASSERT(false, "Could not load font");
		}

		FT_Set_Pixel_Sizes(face, 0, 78); //This could be done on a text box or something
	}

	TextSystem::~TextSystem(){
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void TextSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			d2::TransformComponent* transform = std::get<d2::TransformComponent*>(componentTuple);
			TextComponent* fontComp = std::get<TextComponent*>(componentTuple);

			//TODO: Using hard coded values for now - needs transform component

			///*const*/ math::Matrix4f model = math::Matrix4f(1.0f); /*transform->getWorldTransformMatrix();*/
			
			const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
			const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;
			const auto spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

			std::string text = "Hello, World! AV\n"; //TODO: Need to sort out spaces

			float cursorPos = -550.0f;
			for(auto stringIter = text.begin(); stringIter != text.end(); ++stringIter){
				int8 c = *stringIter;

				FT_Load_Char(face, c, FT_LOAD_RENDER);

				//For spaces we just skip and proceed
				if(face->glyph->bitmap.buffer){
					const auto bearing = math::Vector2f{ face->glyph->bitmap_left, face->glyph->bitmap_top };

					const float width = face->glyph->bitmap.width;
					const float height = face->glyph->bitmap.rows;

					const float x = bearing.x;
					const float y = (height - bearing.y);

					const float xpos = cursorPos + x;
					const float ypos = -y;

					auto texture = gfx::BindableFactory::createTexture(face->glyph->bitmap.buffer, width, height, gfx::TBP_Albedo, gfx::TextureUsage::Font);

					math::Matrix4f model = math::Matrix4f(1.0f);
					model = math::translate(math::Matrix4f(1.0f), { xpos, ypos, 0.0f });
					model *= math::scale(math::Matrix4f(1.0f), { width, height, 0.0f });

					auto character = std::make_shared<gfx::Sprite>(texture);
					character->setModelData(spriteProj * model);

					gfx::Renderer2D::submitCharacter(character);
				}
				
				cursorPos += (face->glyph->advance.x >> 6); //shift by 6 because the advance is 1/64
			}
		}
	}
}