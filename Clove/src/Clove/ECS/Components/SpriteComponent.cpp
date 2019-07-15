#include "clvpch.hpp"
#include "SpriteComponent.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs{
	SpriteComponent::SpriteComponent() = default;

	SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept = default;

	SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept = default;

	SpriteComponent::~SpriteComponent() = default;

	void SpriteComponent::setTexture(const std::string& path){
		Renderable2DComponent::setTexture(gfx::BindableFactory::createTexture(path, gfx::TBP_Sprite));
	}
}