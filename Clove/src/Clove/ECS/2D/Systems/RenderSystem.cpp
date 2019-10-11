#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer2D.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"


namespace clv::ecs::d2{
	RenderSystem::RenderSystem(){
		//Proj
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const math::Matrix4f modelData = transform->getWorldTransformMatrix();
			renderable->sprite->setModelData(spriteProj * modelData);

			gfx::Renderer2D::submitSprite(renderable->sprite);
		}
	}
}