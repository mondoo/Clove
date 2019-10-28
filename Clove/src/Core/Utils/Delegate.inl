namespace clv::utl{
	template<int amount> struct placeholderSequence{ static placeholderSequence ph; };
	template<int amount> placeholderSequence<amount> placeholderSequence<amount>::ph;

	template<typename RetType, typename ObjectType, typename ...Args>
	constexpr auto getArgumentCount(RetType(ObjectType::*)(Args...)){
		return std::integral_constant<size_t, sizeof ...(Args)>{};
	}

	template<typename BindFunctionPrototype, typename ObjectType, int32 ...indices>
	auto doBind(BindFunctionPrototype&& function, ObjectType* object, std::integer_sequence<int32, indices...>){
		return std::bind(std::forward<BindFunctionPrototype>(function), object, placeholderSequence<indices + 1>::ph...);
	}
}

namespace std{
	template<int num> struct is_placeholder<clv::utl::placeholderSequence<num>> : std::integral_constant<int32_t, num>{};
}

namespace clv::utl{
	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype, typename ObjectType>
	void SingleCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function, ObjectType* object){
		constexpr size_t argumentCount = decltype(getArgumentCount(function))::value;
		functionPointer = doBind(std::forward<BindFunctionPrototype>(function), object, std::make_integer_sequence<int32, argumentCount>());
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function){
		functionPointer = function;
	}

	template<typename FunctionPrototype>
	void SingleCastDelegate<FunctionPrototype>::unbind(){
		functionPointer = nullptr;
	}

	template<typename FunctionPrototype>
	bool SingleCastDelegate<FunctionPrototype>::isBound() const{
		return functionPointer;
	}

	template<typename FunctionPrototype>
	SingleCastDelegate<FunctionPrototype>::operator bool() const{
		return isBound();
	}

	template<typename FunctionPrototype>
	template<typename ...Args>
	auto SingleCastDelegate<FunctionPrototype>::broadcast(Args&& ...args) const{
		return functionPointer(std::forward<Args>(args)...);
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype, typename ObjectType>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function, ObjectType* object){
		constexpr size_t argumentCount = decltype(getArgumentCount(function))::value;
		auto functionPointer = doBind(std::forward<BindFunctionPrototype>(function), object, std::make_integer_sequence<int, argumentCount>());
		
		auto handle = MultiCastDelegateHandle{ nextID++ };
		functionPointers.emplace(std::make_pair(handle, functionPointer));

		return handle;
	}

	template<typename FunctionPrototype>
	template<typename BindFunctionPrototype>
	MultiCastDelegateHandle MultiCastDelegate<FunctionPrototype>::bind(BindFunctionPrototype&& function){
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
	void MultiCastDelegate<FunctionPrototype>::broadcast(Args&& ...args) const{
		for(auto& [handle, function] : functionPointers){
			if(function){
				function(std::forward<Args>(args)...);
			}
		}
	}
}