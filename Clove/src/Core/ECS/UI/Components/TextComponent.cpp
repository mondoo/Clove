#include "Clove/Core/ECS/UI/Components/TextComponent.hpp"

namespace clv::ecs::ui{
	TextComponent::TextComponent(clv::ui::Text text)
		: text(std::move(text)){
	}

	TextComponent::TextComponent(const TextComponent& other) = default;

	TextComponent::TextComponent(TextComponent &&other) = default;

	TextComponent& TextComponent::operator=(const TextComponent& other) = default;

	TextComponent &TextComponent::operator=(TextComponent &&other) = default;

	TextComponent::~TextComponent() = default;

	void TextComponent::setText(std::string text){
		this->text.setText(std::move(text));
	}

	void TextComponent::setSize(uint32 size){
		text.setSize(size);
	}
}