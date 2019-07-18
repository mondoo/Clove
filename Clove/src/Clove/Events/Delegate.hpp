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
		//TODO: Constructors and stuff
		Delegate() = default;
		Delegate(const Delegate& other) = delete; 
		Delegate(Delegate&& other) noexcept = default;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = default;
		~Delegate() = default; //Note we need to delete invoker or we'll leak

		template<typename BindFunctionPrototype, typename ...Args>
		void bind(BindFunctionPrototype&& function, Args&& ...args){
			functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
		}

		template<typename ...Args>
		void broadcast(Args&& ...args){
			functionPointer(std::forward<Args>(args)...);
		}
	};
}

#include "Delegate.inl"