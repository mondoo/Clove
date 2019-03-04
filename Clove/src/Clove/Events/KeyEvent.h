#pragma once

#include "Event.h"

namespace clv{
	class CLV_API KeyEvent : public Event{
		//VARIABLES
	protected:
		int keycode = 0;

		//FUNCTIONS
	public:
		inline int getKeyCode() const{ return keycode; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(int inKeyCode) : keycode(inKeyCode){}
	};

	class CLV_API KeyPressedEvent : public KeyEvent{
		//VARIABLES
	private:
		int repeatCount = 0;

		//FUNCTIONS
	public:
		KeyPressedEvent(int inKeyCode, int inRepeatCount)
			: KeyEvent(inKeyCode)
			, repeatCount(inRepeatCount){
		}

		inline int getRepeateCount() const{ return repeatCount; }

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keycode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class CLV_API KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(int inKeyCode) : KeyEvent(inKeyCode){}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
