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
		template<typename BindFunctionPrototype, typename ...Args>
		void bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
			functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
		}

		template<typename BindFunctionPrototype>
		void bindLambda(BindFunctionPrototype&& function){
			functionPointer = function;
		}

		void unbind(){
			functionPointer = nullptr;
		}

		template<typename ...Args>
		auto broadcast(Args&& ...args){
			if(functionPointer){
				return functionPointer(std::forward<Args>(args)...);
			}
		}
	};
}

#include "Delegate.inl"