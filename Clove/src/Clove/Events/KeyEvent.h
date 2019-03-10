#pragma once

#include "Event.h"

namespace clv{
	class CLV_API KeyEvent : public Event{
		//VARIABLES
	protected:
		int keyCode = 0;

		//FUNCTIONS
	public:
		inline int getKeyCode() const{ return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(int inKeyCode) 
			: keyCode(inKeyCode){
		}
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
			ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class CLV_API KeyTypedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyTypedEvent(int inKeyCode)
			: KeyEvent(inKeyCode){
		}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class CLV_API KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(int inKeyCode) 
			: KeyEvent(inKeyCode){
		}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
