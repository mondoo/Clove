#pragma once

#include <queue>

namespace clv::evt{
	//TODO:
	//Move all into inl

	//I'm still 100% happy with this. I feel like it shouldn't be a global signleton
	//-What if people want to use dispatchers with specific situations?
	//-What if people want a dispatcher to not process every frame?
	//I think I should make this object based and have an engine side dispatcher
	//Question then becomes, how do the classes get access to it?

	//I'm happy with how it works just not how to use it

	enum class DispatchType{
		immediate,
		deferred,
	};

	enum class HandledType{
		handled_continue,
		handled_stop,
		unhandled
	};

	//Do I need this or just take the std::function?
	template<typename EventType>
	class Listener{
		//VARIABLES
	/*private:*/
	public:
		SingleCastDelegate<HandledType(EventType&)> del;
	};

	class InternalEventDispatcherBase{
	public:
		virtual ~InternalEventDispatcherBase() = default;
		virtual void processEventQueue() = 0;
	};

	template<typename EventType>
	class InteralEventDispatcher : public InternalEventDispatcherBase{
		//VARIABLES
	private:
		std::queue<EventType> eventQueue;
		std::vector<Listener<EventType>> listeners;

		static bool initialised;
		static InteralEventDispatcher<EventType>* instance;

		//FUNCTIONS
	public:
		InteralEventDispatcher(){
			initialised = true;
			instance = this;
		}

		virtual ~InteralEventDispatcher(){
			//Doesn't look like this is ever called

			CLV_DEBUG_BREAK;

		}

		static bool isInitialised(){
			return initialised;
		}

		static InteralEventDispatcher<EventType>* get(){
			return instance;
		}

		template<typename Function, typename Object>
		void bind(Function&& function, Object* object){
			//bind lambdas?

			Listener<EventType> listener{};
			listener.del.bind(std::forward<Function>(function), object);
			listeners.emplace_back(std::move(listener));
		}

		//unbind?

		void dispatch(EventType&& event, DispatchType dispatchType){
			if(dispatchType == DispatchType::deferred){
				eventQueue.push(event);
			} else{
				doDispatch(std::forward<EventType>(event));
			}
		}

		virtual void processEventQueue() override{
			while(!eventQueue.empty()){
				doDispatch(eventQueue.front());
				eventQueue.pop();
			}
		}

	private:
		template<typename EventType>
		void doDispatch(EventType&& event){
			for(auto& listener : listeners){
				const HandledType handledState = listener.del.broadcast(event);
				if(handledState == HandledType::handled_stop){
					break;
				}
			}
		}
	};

	template<typename EventType> bool InteralEventDispatcher<EventType>::initialised = false;
	template<typename EventType> InteralEventDispatcher<EventType>* InteralEventDispatcher<EventType>::instance = nullptr;


	class EventDispatcher{
		//VARIABLES
	private:
		static std::vector<std::unique_ptr<InternalEventDispatcherBase>> dispatchers;

		//FUNCTIONS
	public:
		template<typename EventType, typename Function, typename Object>
		static void bind(Function&& function, Object* object){
			if(!InteralEventDispatcher<EventType>::isInitialised()){
				dispatchers.emplace_back(std::make_unique<InteralEventDispatcher<EventType>>());
			}
			InteralEventDispatcher<EventType>::get()->bind(std::forward<Function>(function), object);
		}

		//unbind?

		template<typename EventType>
		static void dispatch(EventType&& event, DispatchType dispatchType){
			if(InteralEventDispatcher<EventType>::isInitialised()){
				InteralEventDispatcher<EventType>::get()->dispatch(std::forward<EventType>(event), dispatchType);
			}
		}

		static void processEventQueue(){
			for(auto& dispatcher : dispatchers){
				dispatcher->processEventQueue();
			}
		}
	};
}

#include "Event.inl"