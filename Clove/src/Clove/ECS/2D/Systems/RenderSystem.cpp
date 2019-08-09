#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"

namespace clv::ecs::d2{
	RenderSystem::RenderSystem(){
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
		
		spriteMesh = std::make_shared<gfx::Mesh>(bufferData, indices, gfx::ShaderStyle::_2D);

		//Proj
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		gfx::Renderer::setSpriteMesh(spriteMesh);

		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const math::Matrix4f modelData = transform->getWorldTransformMatrix();
			renderable->sprite->getMaterial().setData(gfx::BBP_2DData, spriteProj * modelData, gfx::ShaderType::Vertex);

			gfx::Renderer::submitSprite(renderable->sprite);
		}
	}
}