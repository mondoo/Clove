#pragma once

#include <queue>

namespace clv{
	/*
	Events in Clove are currently blocking, meaning when an event occurs it
	immediately gets dispatched and must be dealt with right then and there.
	For the future, a better strategy might be to buffer events in an event
	bus and process them during the "event" part of the update stage.
	*/

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
		#define EVENT_CLASS_TYPE(type)	virtual const char* getName() const override { return #type; }
	#else
		#define EVENT_CLASS_TYPE(type)	virtual const char* getName() const override { return #type; }
	#endif
	
	//Macro containing overrides that deal with the event category
	#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	enum class DispatchType{
		deferred,
		immediate
	};

	class Event2{

	};

	template<typename EventType>
	class Listener2{
		//VARIABLES
	private:
		evt::SingleCastDelegate<bool(EventType)> delegate;

		//FUNCTIONS
	public:
		Listener2(evt::SingleCastDelegate<bool(EventType)> delegate)
			: delegate(delegate){

		}

		template<typename ...Args>
		void operator ()(Args&& ...args){
			delegate.broadcast(std::forward<Args>(args)...);
		}
	};

	class EventDispatcher2{
		//VARIABLES
	private:
		std::queue<Event2> eventqueue;

		//how to store the listeners?
		//-will it really just loop through a second array?
		//-is there a way I can put these in a map?

		//The listeners will be forever stored (until they remove themselves)
		//when an event is added that gets put on the queue
		//we then process the queue we loop through the listeners for that event
		//break on the first one that handles it

		//FUNCTIONS
	public:
		template<typename EventType, typename FunctionPrototye, typename ObjectType>
		void bindToEvent(FunctionPrototye func, ObjectType* obj){
			evt::SingleCastDelegate<bool(EventType) del;
			del.bind(func, obj);
			Listener2<EventType> listener(std::move(del));
			//put listener to some container
			//add event type to queue
		}

		template<typename EventType>
		void disptachEvent(EventType event, DispatchType disptachType){
			switch(disptachType){
				case DispatchType::deferred:
					eventqueue.push(event);
					break;
				
				case DispatchType::immediate:
					doDispatch(event);
					break;
				
				default:
					break;
			}
		}

		void processEventQueue(){
			while(!eventqueue.empty()){
				doDispatch(eventqueue.back());
				eventqueue.pop();
			}
		}

	private:
		void doDispatch(Event2 event){
			//go through listeners for event type
		}
	};

	class Event{
		friend class EventDispatcher;

		//VARIABLES
	protected:
		bool handled = false;

		//FUNCTIONS
	public:
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