namespace clv::evt{
	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype, typename ...Args>
	void SingleCastDelegate<FunctionPrototype>::bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
		functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::bindLambda(BindFunctionPrototype&& function){
		functionPointer = function;
	}

	template<typename FunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::unbind(){
		functionPointer = nullptr;
	}

	template<typename FunctionPrototype>
	template<typename ...Args>
	auto SingleCastDelegate<FunctionPrototype>::broadcast(Args&& ...args){
		if(functionPointer){
			return functionPointer(std::forward<Args>(args)...);
		}
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype, typename ...Args>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
		auto handle = MultiCastDelegateHandle{ nextID++ };
		auto functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
		functionPointers.emplace(std::make_pair(handle, functionPointer));
		return handle;
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bindLambda(BindFunctionPrototype&& function){
		auto handle = MultiCastDelegateHandle{ nextID++ };
		functionPointers.emplace(std::make_pair(handle, function));
		return handle;
	}

	template<typename FunctionPrototype>
	void MultiCastDelegate<FunctionPrototype>::unbind(const MultiCastDelegateHandle& handle){
		functionPointers.erase(handle);
	}

	template<typename FunctionPrototype>
	void MultiCastDelegate<FunctionPrototype>::unbindAll(){
		functionPointers.clear();
	}

	template<typename FunctionPrototype>
	template<typename ...Args>
	void MultiCastDelegate<FunctionPrototype>::broadcast(Args&& ...args){
		for(auto& [handle, function] : functionPointers){
			if(function){
				function(std::forward<Args>(args)...);
			}
		}
	}
}