#pragma once

#include "Clove/Core.h"

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
			KeyPressed, KeyReleased,
			Char,
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
#define EVENT_CLASS_TYPE(type)	static EventType::Type getStaticType() { return EventType::##type; }\
								virtual EventType::Type getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

//Macro containing overrides that deal with the event category
#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class CLV_API Event{
		friend class EventDispatcher;

		//VARIABLES
	protected:
		bool handled = false;

		//FUNCTIONS
	public:
		virtual EventType::Type getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		inline bool isHandled() const{ return handled; }

		virtual std::string toString() const{ return getName(); }

		inline bool isInCategory(EventCategory::Type category){
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher{
		template<typename EventType>
		using EventReceiverFn = std::function<bool(EventType&)>;

		//VARIABLES
	private:
		Event& event;

		//FUNCTIONS
	public:
		EventDispatcher(Event& inEvent) : event(inEvent) {}

		template<typename EventType>
		bool dispatch(EventReceiverFn<EventType> func){
			if(event.getEventType() == EventType::getStaticType()){
				event.handled = func(*static_cast<EventType*>(&event));
				return true;
			} else{
				return false;
			}
		}
	};

	inline std::ostream& operator <<(std::ostream& os, const Event& e){
		return os << e.toString();
	}
}

