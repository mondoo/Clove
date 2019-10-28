#include "RenderableComponent.hpp"

#include "Core/Graphics/Sprite.hpp"

namespace clv::ecs::_2D{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setSprite(const std::shared_ptr<gfx::Sprite>& sprite){
		this->sprite = sprite;
	}
}