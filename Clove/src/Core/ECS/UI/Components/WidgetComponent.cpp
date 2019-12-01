#include "Clove/Core/ECS/UI/Components/WidgetComponent.hpp"

namespace clv::ecs::ui{
	WidgetComponent::WidgetComponent() = default;

	WidgetComponent::WidgetComponent(WidgetComponent&& other) noexcept = default;

	WidgetComponent& WidgetComponent::operator=(WidgetComponent&& other) noexcept = default;

	WidgetComponent::~WidgetComponent() = default;

	void WidgetComponent::setSprite(const std::shared_ptr<gfx::Sprite>& sprite){
		this->sprite = sprite;
	}
}