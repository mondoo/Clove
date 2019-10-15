#include "RenderableComponent.hpp"

#include "Clove/Graphics/Sprite.hpp"

namespace clv::ecs::d2{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setSprite(const std::shared_ptr<gfx::Sprite>& sprite){
		this->sprite = sprite;
	}
}