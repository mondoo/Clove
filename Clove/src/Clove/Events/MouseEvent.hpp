#pragma once

#include "Event.hpp"

#include "Clove/Input/MouseButtonCodes.hpp"

namespace clv{
	class MouseMovedEvent : public Event{
		//VARIABLES
	private:
		int mouseX = 0;
		int mouseY = 0;

		//FUNCTIONS
	public:
		MouseMovedEvent(int x, int y);

		int getX() const;
		int getY() const;

		virtual std::string toString() const override;

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

		float getXOffset() const;
		float getYOffset() const;

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class MouseButtonEvent : public Event{
		//VARIABLES
	protected:
		MouseButton button;

		//FUNCTIONS
	public:
		inline MouseButton getMouseButton() const;

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)

	protected:
		MouseButtonEvent(MouseButton inButton);
	};

	class MouseButtonPressedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonPressedEvent(MouseButton inButton);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonReleasedEvent(MouseButton inButton);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}