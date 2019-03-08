#pragma once

#include "Event.h"

namespace clv{
	class CLV_API CharEvent : public Event{
		//VARIABLES
	private:
		unsigned int character = 0;

		//FUNCTIONS	
	public:
		CharEvent(unsigned int inCharacter)
			: character(inCharacter){
		}

		inline unsigned int getChar() const{ return character; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)
		EVENT_CLASS_TYPE(Char)
	};

	class CLV_API KeyEvent : public Event{
		//VARIABLES
	protected:
		int keyCode = 0;
		int scanCode = 0;

		//FUNCTIONS
	public:
		inline int getKeyCode() const{ return keyCode; }
		inline int getScanCode() const{ return scanCode; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(int inKeyCode, int inScanCode) 
			: keyCode(inKeyCode)
			, scanCode(inScanCode){
		}
	};

	class CLV_API KeyPressedEvent : public KeyEvent{
		//VARIABLES
	private:
		int repeatCount = 0;

		//FUNCTIONS
	public:
		KeyPressedEvent(int inKeyCode, int inScanCode, int inRepeatCount)
			: KeyEvent(inKeyCode, inScanCode)
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

	class CLV_API KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(int inKeyCode, int inScanCode) 
			: KeyEvent(inKeyCode, inScanCode){
		}

		virtual std::string toString() const override{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
