#include "RenderSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/2D/Components/TransformComponent.hpp"
#include "Core/ECS/2D/Components/SpriteComponent.hpp"
#include "Core/Graphics/Renderer2D.hpp"
#include "Core/Graphics/Renderables/Sprite.hpp"

namespace clv::ecs::_2D{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		auto componentTuples = manager->getComponentSets<TransformComponent, SpriteComponent>();
		for(auto& tuple : componentTuples){
			TransformComponent* transform = std::get<TransformComponent*>(tuple);
			SpriteComponent* renderable = std::get<SpriteComponent*>(tuple);

			const math::Matrix4f modelData = transform->getWorldTransformMatrix();
			renderable->sprite->setModelData(gfx::Renderer2D::getSpriteProjection() * modelData);

			gfx::Renderer2D::submitSprite(renderable->sprite);
		}
	}
}