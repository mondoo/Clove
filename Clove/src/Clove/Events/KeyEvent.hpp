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
		CLV_API inline Key getKey() const;

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
		CLV_API KeyPressedEvent(Key inKey, int inRepeatCount);

		CLV_API inline int getRepeateCount() const;

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyTypedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		CLV_API KeyTypedEvent(Key inKey);

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class KeyReleasedEvent : public KeyEvent{
		//FUNCTIONS
	public:
		CLV_API KeyReleasedEvent(Key inKey);

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
