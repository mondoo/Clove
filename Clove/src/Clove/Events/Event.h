#pragma once

namespace clv{
	/*
	Events in Clove are currently blocking, meaning when an event occurs it
	immediately gets dispatched and must be dealt with right then and there.
	For the future, a better strategy might be to buffer events in an event
	bus and process them during the "event" part of the update stage.
	*/

	namespace EventType{
		enum Type{
			None = 0,
			WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
			AppTick, AppUpdate, AppRender,
			KeyPressed, KeyReleased, KeyTyped,
			MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
		};
	}

	namespace EventCategory{
		enum Type{
			None = 0,
			EventCategoryApplication	= BIT(0),
			EventCategoryInput			= BIT(1),
			EventCategoryKeyboard		= BIT(2),
			EventCategoryMouse			= BIT(3),
			EventCategoryMouseButton	= BIT(4)
		};
	}

//Macro containing overrides that deal with the event type
#define EVENT_CLASS_TYPE(type)	CLV_API static EventType::Type getStaticType() { return EventType::##type; }\
								CLV_API virtual EventType::Type getEventType() const override { return getStaticType(); }\
								CLV_API virtual const char* getName() const override { return #type; }

//Macro containing overrides that deal with the event category
#define EVENT_CLASS_CATEGORY(category) CLV_API virtual int getCategoryFlags() const override { return category; }

	class Event{
		friend class EventDispatcher;

		//VARIABLES
	protected:
		bool handled = false;

		//FUNCTIONS
	public:
		CLV_API virtual EventType::Type getEventType() const = 0;
		CLV_API virtual const char* getName() const = 0;
		CLV_API virtual int getCategoryFlags() const = 0;

		CLV_API inline bool isHandled() const;

		CLV_API virtual std::string toString() const;

		CLV_API inline bool isInCategory(EventCategory::Type category);
	};

	class EventDispatcher{
		template<typename EventType>
		using EventReceiverFn = std::function<bool(EventType&)>;

		//VARIABLES
	private:
		Event& event;

		//FUNCTIONS
	public:
		CLV_API EventDispatcher(Event& inEvent);

		template<typename EventType>
		bool dispatch(EventReceiverFn<EventType> func);
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e);
}

#include "Clove/Events/Event.inl"