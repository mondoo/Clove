#include "Tunic/ECS/UI/Components/TextComponent.hpp"

namespace tnc::ecs::ui{
	TextComponent::TextComponent(rnd::Text text)
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

	void TextComponent::setSize(clv::uint32 size){
		text.setSize(size);
	}
}