namespace clv{
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