#include "clvpch.hpp"
#include "FontSystem.hpp"

//Should all be temp
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Material.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

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

			const math::Matrix4f model = transform->getWorldTransformMatrix();
			
			const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
			const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;
			const auto spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
			
			//sprite->setModelData(spriteProj * model);

			//gfx::Renderer::submitSprite(sprite);

			/*
			I'll need to construct the vertex buffer in here by the looks of things.

			To get an MVP of font rendering set up I need to put in custom texture coords
			-Might need to refactor the renderer into a 2D (if not now then at least before this gets checked in)
			
			TODO:
				-set it up as a mesh for now (just make one every frame or something)
				-prove out the font rendering basics
				-convert it to a proper system
			*/

			//VB
			gfx::VertexLayout layout;
			layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
			gfx::VertexBufferData bufferData(std::move(layout));
			bufferData.emplaceBack(math::Vector2f{ -1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ 1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(math::Vector2f{ -1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(math::Vector2f{ 1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

			//IB
			std::vector<uint32> indices = {
					1, 3, 0,
					3, 2, 0
			};

			auto material = std::make_shared<gfx::Material>(gfx::ShaderStyle::_2D);
			material->setAlbedoTexture("res/Textures/Zombie-32x32.png");
			material->setData(gfx::BBP_2DData, spriteProj * model, gfx::ShaderType::Vertex);

			auto instance = material->createInstance();

			auto spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, instance);

			gfx::Renderer::submitMesh(spriteMesh);
		}
	}
}