#pragma once

#include "Event.h"
#include "Clove/KeyCodes.h"

namespace clv{
	class CLV_API KeyEvent : public Event{
		//VARIABLES
	protected:
		Key key = Key::_None;

		//FUNCTIONS
	public:
		inline Key getKey() const{ return key; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(Key inKey)
			: key(inKey){
		}
	};

	class CLV_API KeyPressedEvent : public KeyEvent{
		//VARIABLES
	private:
		int repeatCount = 0;

		//FUNCTIONS
	public:
		KeyPressedEvent(Key inKey, int inRepeatCount)
			: KeyEvent(inKey)
			, repeatCount(inRepeatCount){
		}

		inline int getRepeateCount() const{ return repeatCount; }

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << key << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class CLV_API KeyTypedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyTypedEvent(Key inKey)
			: KeyEvent(inKey){
		}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << key;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class CLV_API KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(Key inKey)
			: KeyEvent(inKey){
		}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << key;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
