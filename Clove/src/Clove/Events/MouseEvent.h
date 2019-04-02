#pragma once

#include "Clove/Events/Event.h"
#include "Clove/Input/MouseButtonCodes.h"

namespace clv{
	class MouseMovedEvent : public Event{
		//VARIABLES
	private:
		float mouseX = 0;
		float mouseY = 0;

		//FUNCTIONS
	public:
		MouseMovedEvent(float x, float y);

		CLV_API inline float getX() const;
		CLV_API inline float getY() const;

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class MouseScrolledEvent : public Event{
		//VARIABLES
	private:
		float xOffset = 0;
		float yOffset = 0;

		//FUNCTIONS
	public:
		MouseScrolledEvent(float inXOffset, float inYOffset);

		CLV_API inline float getXOffset() const;
		CLV_API inline float getYOffset() const;
		
		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class MouseButtonEvent : public Event{
		//VARIABLES
	protected:
		MouseButton button;

		//FUNCTIONS
	public:
		CLV_API inline MouseButton getMouseButton() const;

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)

	protected:
		MouseButtonEvent(MouseButton inButton);
	};

	class MouseButtonPressedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonPressedEvent(MouseButton inButton);

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonReleasedEvent(MouseButton inButton);

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

#include "Clove/Events/MouseEvent.inl"