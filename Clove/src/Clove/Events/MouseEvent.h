#pragma once

#include "Event.h"

#include <sstream>

namespace clv{
	class CLV_API MouseMovedEvent : public Event{
		//VARIABLES
	private:
		float mouseX = 0;
		float mouseY = 0;

		//FUNCTIONS
	public:
		MouseMovedEvent(float x, float y)
			: mouseX(x)
			, mouseY(y){
		}

		inline float getX() const{ return mouseX; }
		inline float getY() const{ return mouseY; }

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mouseX << " , " << mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class CLV_API MouseScrolledEvent : public Event{
		//VARIABLES
	private:
		float xOffset = 0;
		float yOffset = 0;

		//FUNCTIONS
	public:
		MouseScrolledEvent(float inXOffset, float inYOffset)
			: xOffset(inXOffset)
			, yOffset(inYOffset){
		}

		inline float getXOffset() const{ return xOffset; }
		inline float getYOffset() const{ return yOffset; }
		
		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << getXOffset() << " , " << getYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class CLV_API MouseButtonEvent : public Event{
		//VARIABLES
	protected:
		int button;

		//FUNCTIONS
	public:
		inline int getMouseButton() const{ return button; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)

	protected:
		MouseButtonEvent(int inButton) : button(inButton){}
	};

	class CLV_API MouseButtonPressedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonPressedEvent(int inButton) : MouseButtonEvent(inButton){}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class CLV_API MouseButtonReleasedEvent : public MouseButtonEvent{
		//FUNCTIONS
	public:
		MouseButtonReleasedEvent(int inButton) : MouseButtonEvent(inButton){}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

