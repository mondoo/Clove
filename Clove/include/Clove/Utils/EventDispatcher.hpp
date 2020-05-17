#pragma once

namespace clv::utl {
	using ListenerId = size_t;
	static constexpr ListenerId invalidListenerId = 0;

	template<typename EventType>
	using ListenerFunction = std::function<void(EventType&)>;

	struct EventHandle{
		//friend class EventManager::EventContainerBase;

	private:
		ListenerId id = invalidListenerId;
		std::function<void(ListenerId)> clearEvent;

	public:
		EventHandle(ListenerId id, std::function<void(ListenerId)> clearFunc, class EventContainerBase* container);

		EventHandle(const EventHandle& other) = delete;
		
		EventHandle& operator=(const EventHandle& other) = delete;
		
		~EventHandle(){
			reset();
		}

		void reset(){
			if(isValid()) {
				clearEvent(id);

				clear();
			}
		}
		bool isValid() const{
			return clearEvent != nullptr && id != invalidListenerId;
		}

		operator ListenerId() const{
			return id;
		}

	//private:
		void clear(){
			clearEvent = nullptr;
			id = invalidListenerId;
		}
	};

	//Temp? Would like it inside the manager
	class EventContainerBase {
		friend class EventHandle;

		//VARIABLES
	protected:
		inline static ListenerId nextId = 1;

		std::vector<EventHandle*> handles;
		//std::unordered_map<ListenerId, std::function<void()>> handleClearFuncs;

		//FUNCTI0NS
	public:
		virtual ~EventContainerBase() {
			/*for(auto&& handlePair : handleClearFuncs) {
					handlePair.second();
				}*/

			for(auto* handle : handles) {
				handle->clear();
			}
		}
	};

	class EventManager {
		//TYPES
	private:
		

		template<typename EventType>
		class EventContainer : public EventContainerBase {
			//TYPES
		private:
			using ContainerFunctionType = ListenerFunction<EventType>;
			
			//VARIABLES
		private:
			std::unordered_map<ListenerId, size_t> listenerIdToIndexMap;
			std::vector<ContainerFunctionType> listeners;

			//FUNCTIONS
		public:
			//TODO: Ctors

			EventHandle addListener(ContainerFunctionType function) {
				listeners.push_back(function);

				ListenerId id = nextId++;
				listenerIdToIndexMap[id] = listeners.size() - 1;

				return { id, std::bind(&EventContainer<EventType>::removeListener, this, std::placeholders::_1), this };
			}
			void removeListener(ListenerId id) {
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
		std::unordered_map<size_t, std::unique_ptr<EventContainerBase>> containers;

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