namespace clv{
	inline bool Event::isHandled() const{
		return handled;
	}

	inline bool Event::isInCategory(EventCategory category){
		return getCategoryFlags() & category;
	}

	template<typename EventType>
	bool EventDispatcher::dispatch(EventReceiverFn<EventType> func){
		if(event.getEventType() == EventType::getStaticType()){
			event.handled = func(*static_cast<EventType*>(&event));
			return true;
		} else{
			return false;
		}
	}
}