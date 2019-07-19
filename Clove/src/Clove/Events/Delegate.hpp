#pragma once

//TODO: .inl stuff

namespace clv::evt{//Maybe move to utils?
	template<typename FunctionPrototype>
	class Delegate{
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		Delegate() = default;
		Delegate(const Delegate& other) = delete; 
		Delegate(Delegate&& other) noexcept = default;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = default;
		~Delegate() = default;

		template<typename BindFunctionPrototype, typename ...Args>
		void bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
			functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
		}

		template<typename BindFunctionPrototype>
		void bindLambda(BindFunctionPrototype&& function){
			functionPointer = function;
		}

		template<typename ...Args>
		auto broadcast(Args&& ...args){
			return functionPointer(std::forward<Args>(args)...);
		}
	};
}

#include "Delegate.inl"