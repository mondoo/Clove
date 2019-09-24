#include "clvpch.hpp"
#include "FontSystem.hpp"

//Should all be temp
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Mesh.hpp"
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
	FontSystem::FontSystem(){
		//Just getting something on the screen for now
		//sprite = std::make_shared<gfx::Sprite>("res/Textures/Zombie-32x32.png");

		if(FT_Init_FreeType(&ft)){
			CLV_ASSERT(false, "Could not load freetype");
		}

		if(FT_New_Face(ft, "res/Fonts/Roboto/Roboto-Black.ttf", 0, &face)){
			CLV_ASSERT(false, "Could not load font");
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
	}

	FontSystem::~FontSystem(){
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void FontSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			d2::TransformComponent* transform = std::get<d2::TransformComponent*>(componentTuple);
			FontComponent* fontComp = std::get<FontComponent*>(componentTuple);

			//Construct a sprite out of the font data


			//Using hard coded values for now - needs transform component

			/*const*/ math::Matrix4f model = math::Matrix4f(1.0f); /*transform->getWorldTransformMatrix();*/
			
			const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
			const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;
			const auto spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
			
			//sprite->setModelData(spriteProj * model);

			//gfx::Renderer::submitSprite(sprite);

			//image dimensions
			const float imageSize = 512.0f;

			//106

			/*
			 TODO:
			 	-Now need to render strings of text
			 */

			//const std::string text = "Hello, World!";

			//Parse the file and load the font map
			struct FontData{
				float x = 0.0f;
				float y = 0.0f;
				float width = 0.0f;
				float height = 0.0f;
				float xoffset = 0.0f;
				float yoffset = 0.0f;
				float xadvance = 0.0f;
			};
			std::unordered_map<char, FontData> charMap;


			std::string text = "Hello, World! AV"; //TODO: Need to sort out spaces

			float cursorPos = -550.0f;
			for(auto stringIter = text.begin(); stringIter != text.end(); ++stringIter){
				int8 c = *stringIter;

				FT_Load_Char(face, c, FT_LOAD_RENDER);

				const auto bearing = math::Vector2f{ face->glyph->bitmap_left, face->glyph->bitmap_top };

				const float width = face->glyph->bitmap.width;
				const float height = face->glyph->bitmap.rows;
				
				const float x = bearing.x;
				const float y = (height - bearing.y);

				auto texture = gfx::BindableFactory::createTexture(face->glyph->bitmap.buffer, width, height, gfx::TBP_Albedo, gfx::TextureUsage::Font);

				//VB
				gfx::VertexLayout layout;
				layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
				gfx::VertexBufferData bufferData(std::move(layout));
				//-height because it's easier to draw top down when dealing with the yoffset
				bufferData.emplaceBack(math::Vector2f{ 0,		0 },		math::Vector2f{ 0.0f, 1.0f });	//Bottom left
				bufferData.emplaceBack(math::Vector2f{ width,	0 },		math::Vector2f{ 1.0f, 1.0f });	//Bottom right
				bufferData.emplaceBack(math::Vector2f{ 0,		height },	math::Vector2f{ 0.0f, 0.0f });	//Top left
				bufferData.emplaceBack(math::Vector2f{ width,	height },	math::Vector2f{ 1.0f, 0.0f });	//Top right

				//IB
				const std::vector<uint32> indices = {
					1, 3, 0,
					3, 2, 0
				};

				const float xpos = cursorPos; //TODO: bearing.x
				const float ypos = -y;

				model = math::translate(math::Matrix4f(1.0f), { xpos, ypos, 0.0f });

				auto material = std::make_shared<gfx::Material>(gfx::ShaderStyle::Font);
				material->setAlbedoTexture(texture);
				material->setData(gfx::BBP_2DData, spriteProj * model, gfx::ShaderType::Vertex);

				auto instance = material->createInstance();

				auto spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, instance);

				//TEMP: still renderer for now - needs to be moved to the 2d renderer
				gfx::Renderer::submitMesh(spriteMesh);

				
				cursorPos += (face->glyph->advance.x >> 6); //shift by 6 because the advance is 1/64
			}
		}
	}
}