namespace blb::ecs {
	template<typename FunctionType, typename ObjectType>
	InputComponent::BindingId InputComponent::bind(clv::Key key, FunctionType&& function, ObjectType* object) {
		const BindingId id = nextId++;

		keyBindings[key].emplace_back(std::bind(function, object, std::placeholders::_1));
		const size_t index = keyBindings[key].size() - 1;

		keyIdToIndexMap[key][id] = index;

		return id;
	}

	template<typename FunctionType, typename ObjectType>
	InputComponent::BindingId InputComponent::bind(clv::MouseButton button, FunctionType&& function, ObjectType* object) {
		const BindingId id = nextId++;

		mouseButtonBindings[button].emplace_back(std::bind(function, object, std::placeholders::_1));
		const size_t index = mouseButtonBindings[button].size() - 1;

		buttonIdToIndexMap[button][id] = index;

		return id;
	}

	template<typename FunctionType>
	InputComponent::BindingId InputComponent::bind(clv::Key key, FunctionType&& function) {
		const BindingId id = nextId++;

		keyBindings[key].emplace_back(function);
		const size_t index = keyBindings[key].size() - 1;

		keyIdToIndexMap[key][id] = index;

		return id;
	}

	template<typename FunctionType>
	InputComponent::BindingId InputComponent::bind(clv::MouseButton button, FunctionType&& function) {
		const BindingId id = nextId++;

		mouseButtonBindings[button].emplace_back(function);
		const size_t index = mouseButtonBindings[button].size() - 1;

		buttonIdToIndexMap[button][id] = index;

		return id;
	}
}