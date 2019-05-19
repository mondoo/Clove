#pragma once

#include "Event.hpp"

#include "Clove/Input/KeyCodes.hpp"

namespace clv{
	class KeyEvent : public Event{
		//VARIABLES
	protected:
		Key key = Key::None;

		//FUNCTIONS
	public:
		inline Key getKey() const;

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)

	protected:
		KeyEvent(Key inKey);
	};

	class KeyPressedEvent : public KeyEvent{
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

	class KeyTypedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyTypedEvent(Key inKey);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		KeyReleasedEvent(Key inKey);

		virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
