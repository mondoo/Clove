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

			const math::Matrix4f model = math::Matrix4f(1.0f); /*transform->getWorldTransformMatrix();*/
			
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

			const float x = 0.0f;
			const float y = 0.0f;
			const float width = 78.0f;
			const float height = 80.0f;

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
			bufferData.emplaceBack(math::Vector2f{ -halfWidth_text, -halfHeight_text }, bottomLeft); //Bottom left
			bufferData.emplaceBack(math::Vector2f{ halfWidth_text, -halfHeight_text }, bottomRight); //Bottom right
			bufferData.emplaceBack(math::Vector2f{ -halfWidth_text, halfHeight_text }, topLeft); //Top left
			bufferData.emplaceBack(math::Vector2f{ halfWidth_text, halfHeight_text }, topRight); //Top right

			//IB
			std::vector<uint32> indices = {
				1, 3, 0,
				3, 2, 0
			};

			auto material = std::make_shared<gfx::Material>(gfx::ShaderStyle::_2D);
			//We do not want to flip the texture
			material->setAlbedoTexture(gfx::BindableFactory::createTexture("res/Fonts/fnt/arial.png", gfx::TBP_Albedo, false));
			material->setData(gfx::BBP_2DData, spriteProj * model, gfx::ShaderType::Vertex);

			auto instance = material->createInstance();

			auto spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, instance);

			//TEMP: still renderer for now - needs to be moved to the 2d renderer
			gfx::Renderer::submitMesh(spriteMesh);
		}
	}
}