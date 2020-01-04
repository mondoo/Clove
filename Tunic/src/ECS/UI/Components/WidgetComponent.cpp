#include "Tunic/ECS/UI/Components/WidgetComponent.hpp"

namespace tnc::ecs::ui{
	WidgetComponent::WidgetComponent() = default;

	WidgetComponent::WidgetComponent(const WidgetComponent& other) = default;

	WidgetComponent::WidgetComponent(WidgetComponent&& other) noexcept = default;

	WidgetComponent& WidgetComponent::operator=(const WidgetComponent& other) = default;

	WidgetComponent& WidgetComponent::operator=(WidgetComponent&& other) noexcept = default;

	WidgetComponent::~WidgetComponent() = default;

	void WidgetComponent::setSprite(const std::shared_ptr<clv::gfx::Sprite>& sprite){
		this->sprite = sprite;
	}
}