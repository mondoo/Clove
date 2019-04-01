#pragma once

#include "Clove/Events/Event.h"
#include "Clove/Input/KeyCodes.h"

namespace clv{
	class CLV_API KeyEvent : public Event{
		//VARIABLES
	protected:
		Key key = Key::_None;

		//FUNCTIONS
	public:
		inline Key getKey() const;

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(Key inKey);
	};

	class CLV_API KeyPressedEvent : public KeyEvent{
		//VARIABLES
	private:
		int repeatCount = 0;

		//FUNCTIONS
	public:
		KeyPressedEvent(Key inKey, int inRepeatCount);

		inline int getRepeateCount() const;

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class CLV_API KeyTypedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyTypedEvent(Key inKey);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class CLV_API KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(Key inKey);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
