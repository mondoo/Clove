#pragma once

namespace clv::utl {
	using ListenerId = size_t;
	static constexpr ListenerId invalidListenerId = 0;

	template<typename EventType>
	using ListenerFunction = std::function<void(EventType&)>;

	struct EventHandle{
	private:
		ListenerId id = invalidListenerId;
		std::function<void(ListenerId)> clearEvent;

	public:
		EventHandle(ListenerId id, std::function<void(ListenerId)> clearFunc)
			: id(id)
			, clearEvent(std::move(clearFunc)) {
		}

		EventHandle(const EventHandle& other) = delete;
		
		EventHandle& operator=(const EventHandle& other) = delete;
		
		~EventHandle(){
			reset();
		}

		void reset(){
			if(isValid()) {
				clearEvent(id);

				clearEvent = nullptr;
				id = invalidListenerId;
			}
		}

		//TODO: invalidate handle when the event itself goes out of scope

		bool isValid() const{
			return clearEvent != nullptr && id != invalidListenerId;
		}

		operator ListenerId() const{
			return id;
		}
	};

	class EventManager {
		//TYPES
	private:
		class EventContainerInterface {
			//VARIABLES
		protected:
			inline static ListenerId nextId = 1;

			//FUNCTI0NS
		public:
			virtual ~EventContainerInterface() = default;

			virtual void removeListener(ListenerId id) = 0;
		};

		template<typename EventType>
		class EventContainer : public EventContainerInterface {
			//TYPES
		private:
			using ContainerFunctionType = ListenerFunction<EventType>;
			
			//VARIABLES
		private:
			std::unordered_map<ListenerId, size_t> listenerIdToIndexMap;
			std::vector<ContainerFunctionType> listeners;

			//FUNCTIONS
		public:
			
			EventHandle addListener(ContainerFunctionType function) {
				listeners.push_back(function);

				ListenerId id = nextId++;
				listenerIdToIndexMap[id] = listeners.size() - 1;

				return { id, std::bind(&EventContainer<EventType>::removeListener, this, std::placeholders::_1) };
			}
			void removeListener(ListenerId id) final {
				if(auto iter = listenerIdToIndexMap.find(id); iter != listenerIdToIndexMap.end()) {
					const size_t index = iter->second;
					const size_t lastIndex = listeners.size() - 1;

					std::optional<ListenerId> movedEvent;
					if(index < lastIndex) {
						for(auto&& idToIndexPair : listenerIdToIndexMap) {
							if(idToIndexPair.second == lastIndex) {
								movedEvent = idToIndexPair.first;
								break;
							}
						}

						listeners[iter->second] = listeners.back();
					}
					listeners.pop_back();
					listenerIdToIndexMap.erase(id);

					if(movedEvent) {
						listenerIdToIndexMap[*movedEvent] = index;
					}
				}
			}

			typename std::vector<ContainerFunctionType>::iterator begin() {
				return listeners.begin();
			}
			typename std::vector<ContainerFunctionType>::const_iterator begin() const {
				return listeners.begin();
			}

			typename std::vector<ContainerFunctionType>::iterator end() {
				return listeners.end();
			}
			typename std::vector<ContainerFunctionType>::const_iterator end() const {
				return listeners.end();
			}
		};

		//VARIABLES
	private:
		std::unordered_map<size_t, std::unique_ptr<EventContainerInterface>> containers;

		//FUNCTIONS
	public:
		//TODO: Ctors

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

		template<typename EventType>
		EventHandle bindToEvent(ListenerFunction<EventType>&& function) {
			return manager.getEventContainer<EventType>().addListener(std::forward<ListenerFunction<EventType>>(function));
		}

		template<typename EventType>
		void unbindFromEvent(ListenerId id){
			manager.getEventContainer<EventType>().removeListener(id);
		}

		template<typename EventType>
		void broadCastEvent(EventType& event) {
			for(auto&& func : manager.getEventContainer<EventType>()) {
				func(event);
			}
		}
	};
}