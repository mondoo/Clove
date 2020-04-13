#include "Tunic/UI/Widget.hpp"

namespace tnc::ui {
	void Widget::setPosition(clv::mth::vec2f position) {
		this->position = std::move(position);
	}

	void Widget::setRotation(float rotation) {
		this->rotation = rotation;
	}

	void Widget::setScale(clv::mth::vec2f scale) {
		this->scale = std::move(scale);
	}

	const clv::mth::vec2f& Widget::getPosition() const {
		return position;
	}

	float Widget::getRotation() const {
		return rotation;
	}

	const clv::mth::vec2f& Widget::getScale() const {
		return scale;
	}
}