#include "Tunic/ECS/2D/Components/SpriteComponent.hpp"

#include "Tunic/Rendering/Renderables/Sprite.hpp"

namespace tnc::ecs::_2D{
	SpriteComponent::SpriteComponent() = default;

	SpriteComponent::SpriteComponent(const std::shared_ptr<rnd::Sprite>& sprite)
		: sprite(sprite){
	}

	SpriteComponent::SpriteComponent(const SpriteComponent& other) = default;

	SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept = default;

	SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) = default;

	SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept = default;

	SpriteComponent::~SpriteComponent() = default;

	void SpriteComponent::setSprite(const std::shared_ptr<rnd::Sprite>& sprite){
		this->sprite = sprite;
	}
}