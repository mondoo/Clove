namespace blb::ecs {
	template<typename FunctionType, typename ObjectType>
	InputComponent::BindingId InputComponent::bind(KeyButtonState key, FunctionType&& function, ObjectType* object) {
		const BindingId id = nextId++;

		std::vector<KeyBindingFunction>& keyFunctions = keyBindings[key];

		keyFunctions.emplace_back(std::bind(std::forward<FunctionType>(function), object));
		const size_t index = keyFunctions.size() - 1;

		keyIdToIndexMap[key][id] = index;

		return id;
	}

	template<typename FunctionType, typename ObjectType>
	InputComponent::BindingId InputComponent::bind(MouseButtonState button, FunctionType&& function, ObjectType* object) {
		const BindingId id = nextId++;

		std::vector<MouseBindingFunction>& mouseFunctions = mouseButtonBindings[button];

		mouseFunctions.emplace_back(std::bind(std::forward<FunctionType>(function), object));
		const size_t index = mouseFunctions.size() - 1;

		buttonIdToIndexMap[button][id] = index;

		return id;
	}

	template<typename FunctionType>
	InputComponent::BindingId InputComponent::bind(KeyButtonState key, FunctionType&& function) {
		const BindingId id = nextId++;

		std::vector<KeyBindingFunction>& keyFunctions = keyBindings[key];

		keyFunctions.emplace_back(function);
		const size_t index = keyFunctions.size() - 1;

		keyIdToIndexMap[key][id] = index;

		return id;
	}

	template<typename FunctionType>
	InputComponent::BindingId InputComponent::bind(MouseButtonState button, FunctionType&& function) {
		const BindingId id = nextId++;

		std::vector<MouseBindingFunction>& mouseFunctions = mouseButtonBindings[button];

		mouseFunctions.emplace_back(function);
		const size_t index = mouseFunctions.size() - 1;

		buttonIdToIndexMap[button][id] = index;

		return id;
	}
}