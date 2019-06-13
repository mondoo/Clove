#pragma once

namespace clv{
	/*
	Events in Clove are currently blocking, meaning when an event occurs it
	immediately gets dispatched and must be dealt with right then and there.
	For the future, a better strategy might be to buffer events in an event
	bus and process them during the "event" part of the update stage.
	*/

	enum EventType{
		ET_None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory{
		EC_None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};


	//Macro containing overrides that deal with the event type
	#if CLV_PLATFORM_WINDOWS
		#define EVENT_CLASS_TYPE(type)	static EventType getStaticType() { return EventType::##type; }\
										virtual EventType getEventType() const override { return getStaticType(); }\
										virtual const char* getName() const override { return #type; }
	#else
		#define EVENT_CLASS_TYPE(type)	static EventType getStaticType() { return EventType::type; }\
										virtual EventType getEventType() const override { return getStaticType(); }\
										virtual const char* getName() const override { return #type; }
	#endif
	
	//Macro containing overrides that deal with the event category
	#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event{
		friend class EventDispatcher;

		//VARIABLES
	protected:
		bool handled = false;

		//FUNCTIONS
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		bool isHandled() const;

		virtual std::string toString() const;

		bool isInCategory(EventCategory category);
	};

	class EventDispatcher{
		template<typename EventType>
		using EventReceiverFn = std::function<bool(EventType&)>;

		//VARIABLES
	private:
		Event& event;

		//FUNCTIONS
	public:
		EventDispatcher(Event& inEvent);

		template<typename EventType>
		bool dispatch(EventReceiverFn<EventType> func);
	};

	std::ostream& operator<<(std::ostream& os, const Event& e);
}

#include "Event.inl"