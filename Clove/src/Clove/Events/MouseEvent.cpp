#include "clvpch.hpp"
#include "MouseEvent.hpp"

namespace clv{
	MouseMovedEvent::MouseMovedEvent(int x, int y)
		: mouseX(x)
		, mouseY(y){
	}

	int MouseMovedEvent::getX() const{
		return mouseX;
	}

	int MouseMovedEvent::getY() const{
		return mouseY;
	}

	std::string MouseMovedEvent::toString() const{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mouseX << " , " << mouseY;
		return ss.str();
	}

	MouseScrolledEvent::MouseScrolledEvent(float inXOffset, float inYOffset)
		: xOffset(inXOffset)
		, yOffset(inYOffset){
	}

	float MouseScrolledEvent::getXOffset() const{
		return xOffset;
	}

	float MouseScrolledEvent::getYOffset() const{
		return yOffset;
	}

	std::string MouseScrolledEvent::toString() const{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << getXOffset() << " , " << getYOffset();
		return ss.str();
	}

	inline MouseButton MouseButtonEvent::getMouseButton() const{
		return button;
	}

	MouseButtonEvent::MouseButtonEvent(MouseButton inButton)
		: button(inButton){
	}

	MouseButtonPressedEvent::MouseButtonPressedEvent(MouseButton inButton)
		: MouseButtonEvent(inButton){
	}

	std::string MouseButtonPressedEvent::toString() const{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button;
		return ss.str();
	}

	MouseButtonReleasedEvent::MouseButtonReleasedEvent(MouseButton inButton)
		: MouseButtonEvent(inButton){
	}

	std::string MouseButtonReleasedEvent::toString() const{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << button;
		return ss.str();
	}
}