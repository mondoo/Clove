namespace clv::utl {
	template<typename FunctionPrototype>
	template<typename RetType, typename ObjectType, typename... Args>
	void SingleCastDelegate<FunctionPrototype>::bind(RetType (ObjectType::*function)(Args...), ObjectType* object) {
		functionPointer = [object, function](Args&&... args) -> RetType {
			return (object->*function)(std::forward<Args>(args)...);
		};
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function) {
		functionPointer = function;
	}

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>::SingleCastDelegate() = default;

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>::SingleCastDelegate(SingleCastDelegate&& other) noexcept = default;

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>& SingleCastDelegate<FunctionPrototype>::operator=(SingleCastDelegate&& other) noexcept = default;

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>::~SingleCastDelegate() = default;

	template<typename FunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::unbind() {
		functionPointer = nullptr;
	}

	template<typename FunctionPrototype>
	bool SingleCastDelegate<FunctionPrototype>::isBound() const {
		return functionPointer != nullptr;
	}

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>::operator bool() const {
		return isBound();
	}

	template<typename FunctionPrototype>
	template<typename... Args>
	auto SingleCastDelegate<FunctionPrototype>::broadcast(Args&&... args) const {
		return functionPointer(std::forward<Args>(args)...);
	}

	template<typename FunctionPrototype>
	template<typename RetType, typename ObjectType, typename... Args>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bind(RetType (ObjectType::*function)(Args...), ObjectType* object) {
		auto functionPointer = [object, function](Args&&... args) -> RetType {
			return (object->*function)(std::forward<Args>(args)...);
		};

		auto handle = MultiCastDelegateHandle{ nextId++ };
		functionPointers.emplace(std::make_pair(handle, functionPointer));

		return handle;
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function) {
		auto handle = MultiCastDelegateHandle{ nextId++ };
		functionPointers.emplace(std::make_pair(handle, function));
		return handle;
	}

	template<typename FunctionPrototype>
	MultiCastDelegate<FunctionPrototype>::MultiCastDelegate() = default;

	template<typename FunctionPrototype>
	MultiCastDelegate<FunctionPrototype>::MultiCastDelegate(MultiCastDelegate&& other) noexcept {
		functionPointers = std::move(other.functionPointers);
	}

	template<typename FunctionPrototype>
	MultiCastDelegate<FunctionPrototype>& MultiCastDelegate<FunctionPrototype>::operator=(MultiCastDelegate&& other) noexcept {
		functionPointers = std::move(other.functionPointers);
		return *this;
	}

	template<typename FunctionPrototype>
	MultiCastDelegate<FunctionPrototype>::~MultiCastDelegate() = default;

	template<typename FunctionPrototype>
	void MultiCastDelegate<FunctionPrototype>::unbind(const MultiCastDelegateHandle& handle) {
		functionPointers.erase(handle);
	}

	template<typename FunctionPrototype>
	void MultiCastDelegate<FunctionPrototype>::unbindAll() {
		functionPointers.clear();
	}

	template<typename FunctionPrototype>
	template<typename... Args>
	void MultiCastDelegate<FunctionPrototype>::broadcast(Args&&... args) const {
		for(auto& [handle, function] : functionPointers) {
			if(function) {
				function(std::forward<Args>(args)...);
			}
		}
	}
}
