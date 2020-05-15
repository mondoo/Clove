#pragma once

#include <unordered_map>
#include <cinttypes>

namespace clv::utl {
	class EventContainerInterface{

	};

	template<typename EventType>
	class EventContainer : public EventContainerInterface{
	public:
		using FunctionType = std::function<void(EventType&)>;

		std::vector<FunctionType> listeners; //TODO: have the listeners take the event that's been called (will contain the data)
	};

	class EventManager{
	private:
		std::unordered_map<size_t, std::unique_ptr<EventContainerInterface>> containers;

	public:
		template<typename EventType>
		EventContainer<EventType>& getEventContainer() {
			const size_t eventId = typeid(EventType).hash_code();

			if(containers.find(eventId) == containers.end()) {
				containers[eventId] = std::make_unique<EventContainer<EventType>>();
			}

			return static_cast<EventContainer<EventType>&>(*containers[eventId]);
		}
	};

	class EventDispatcher {
		//VARIABLES
	private:
		EventManager manager;

		//FUNCTIONS
	public:
		//TODO: Ctors

		//TODO: return some type of ID
		template<typename EventType>
		void bindToEvent(typename EventContainer<EventType>::FunctionType function) {
			manager.getEventContainer<EventType>().listeners.push_back(function);
		}

		template<typename EventType>
		void broadCastEvent(EventType& event) {
			for(auto&& func : manager.getEventContainer<EventType>().listeners) {
				func(event);
			}
		}
	};
}