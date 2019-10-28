#include "RenderSystem.hpp"

#include "Core/Graphics/Renderer2D.hpp"
#include "Core/Graphics/Sprite.hpp"
#include "Core/Application.hpp"
#include "Core/Platform/Window.hpp"


namespace clv::ecs::_2D{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const math::Matrix4f modelData = transform->getWorldTransformMatrix();
			renderable->sprite->setModelData(gfx::Renderer2D::getSpriteProjection() * modelData);

			gfx::Renderer2D::submitSprite(renderable->sprite);
		}
	}
}