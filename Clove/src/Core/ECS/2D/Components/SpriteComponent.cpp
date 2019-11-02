#include "SpriteComponent.hpp"

#include "Core/Graphics/Sprite.hpp"

namespace clv::ecs::_2D{
	SpriteComponent::SpriteComponent() = default;

	SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept = default;

	SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept = default;

	SpriteComponent::~SpriteComponent() = default;

	void SpriteComponent::setSprite(const std::shared_ptr<gfx::Sprite>& sprite){
		this->sprite = sprite;
	}
}