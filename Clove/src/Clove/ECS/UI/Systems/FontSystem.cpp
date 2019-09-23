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

namespace clv::ecs::ui{
	FontSystem::FontSystem(){
		//Just getting something on the screen for now
		//sprite = std::make_shared<gfx::Sprite>("res/Textures/Zombie-32x32.png");
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

			//Read file
			std::string rawFontData;
			std::ifstream in("res/Fonts/fnt/arial.fnt", std::ios::in | std::ios::binary);
			CLV_ASSERT(in, "Didn't work!");
			in.seekg(0, std::ios::end);
			rawFontData.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&rawFontData[0], rawFontData.size());
			in.close();

			/*
			do i want the reverse iterator,
			shouldn't just be a case of looping through the interator line by line?
			*/


			std::unordered_map<int32, FontData> fontMap;

			std::stringstream stream(rawFontData);
			std::string line;
			std::getline(stream, line, '\n');
			std::getline(stream, line, '\n');
			std::getline(stream, line, '\n');
			std::getline(stream, line, '\n');

			while (!stream.eof()){
				std::getline(stream, line, '\n'); //Loop through each line

				int32 id;
				FontData data;
				
				std::stringstream lineStream(line);

				while(!lineStream.eof()){
					std::string section;
					std::getline(lineStream, section, ' ');

					if (section.compare("") == 0){
						continue;
					}

					std::stringstream sectionStream(section);
					std::string identifier;
					std::string val;
					std::getline(sectionStream, identifier, '=');
					std::getline(sectionStream, val, ' ');

					//CLV_LOG_DEBUG("current section: {0} = {1}", identifier, val);

					if (identifier == "id"){
						id = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "x"){
						data.x = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "y"){
						data.y = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "width"){
						data.width = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "height"){
						data.height = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "xoffset"){
						data.xoffset = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "yoffset"){
						data.yoffset = std::atoi(val.c_str());
						continue;
					}

					if (identifier == "xadvance"){
						data.xadvance = std::atoi(val.c_str());
						continue;
					}
				}

				/*CLV_LOG_DEBUG("Adding: {0}", id);

				CLV_LOG_DEBUG("    x: {0}", data.x);
				CLV_LOG_DEBUG("    y: {0}", data.y);
				CLV_LOG_DEBUG("    width: {0}", data.width);
				CLV_LOG_DEBUG("    height: {0}", data.height);
				CLV_LOG_DEBUG("    xoffset: {0}", data.xoffset);
				CLV_LOG_DEBUG("    yoffset: {0}", data.yoffset);
				CLV_LOG_DEBUG("    xadvance: {0}", data.xadvance);*/

				fontMap[id] = data;
			}
			

			//size_t pos = rawFontData.find();


			/*while(pos != std::string::npos){

			}*/
			
			//int32 theOne = 'f';

			/*CLV_LOG_DEBUG("the one: {0}", theOne);

			CLV_LOG_DEBUG("    x: {0}", fontMap[theOne].x);
			CLV_LOG_DEBUG("    y: {0}", fontMap[theOne].y);
			CLV_LOG_DEBUG("    width: {0}", fontMap[theOne].width);
			CLV_LOG_DEBUG("    height: {0}", fontMap[theOne].height);
			CLV_LOG_DEBUG("    xoffset: {0}", fontMap[theOne].xoffset);
			CLV_LOG_DEBUG("    yoffset: {0}", fontMap[theOne].yoffset);
			CLV_LOG_DEBUG("    xadvance: {0}", fontMap[theOne].xadvance);*/

			std::string text = "Hello, World!";

			float cursorPos = -550.0f;
			for(auto stringIter = text.begin(); stringIter != text.end(); ++stringIter){
				int8 c = *stringIter;

				const float x = fontMap[c].x;
				const float y = fontMap[c].y;
				const float width = fontMap[c].width;
				const float height = fontMap[c].height;

				const float halfWidth_text = width / 2.0f;
				const float halfHeight_text = height / 2.0f;

				const math::Vector2f topLeft = { x / imageSize, y / imageSize };
				const math::Vector2f topRight = { (x + width) / imageSize, y / imageSize };
				const math::Vector2f bottomLeft = { x / imageSize, (y + height) / imageSize };
				const math::Vector2f bottomRight = { (x + width) / imageSize, (y + height) / imageSize };

				//VB
				gfx::VertexLayout layout;
				layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
				gfx::VertexBufferData bufferData(std::move(layout));
				//-height because it's easier to draw top down when dealing with the yoffset
				bufferData.emplaceBack(math::Vector2f{ 0,		-height },	bottomLeft);	//Bottom left
				bufferData.emplaceBack(math::Vector2f{ width,	-height },	bottomRight);	//Bottom right
				bufferData.emplaceBack(math::Vector2f{ 0,		0 },		topLeft);		//Top left
				bufferData.emplaceBack(math::Vector2f{ width,	0 },		topRight);		//Top right

				//IB
				std::vector<uint32> indices = {
					1, 3, 0,
					3, 2, 0
				};

				const float xpos = cursorPos + fontMap[c].xoffset;
				const float ypos = -fontMap[c].yoffset;

				model = math::translate(math::Matrix4f(1.0f), { xpos, ypos, 0.0f });

				auto material = std::make_shared<gfx::Material>(gfx::ShaderStyle::_2D);
				//We do not want to flip the texture
				material->setAlbedoTexture(gfx::BindableFactory::createTexture("res/Fonts/fnt/arial.png", gfx::TBP_Albedo, false));
				material->setData(gfx::BBP_2DData, spriteProj * model, gfx::ShaderType::Vertex);

				auto instance = material->createInstance();

				auto spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, instance);

				//TEMP: still renderer for now - needs to be moved to the 2d renderer
				gfx::Renderer::submitMesh(spriteMesh);

				
				cursorPos += fontMap[c].xadvance/* + halfWidth_text*/;
			}
		}
	}
}