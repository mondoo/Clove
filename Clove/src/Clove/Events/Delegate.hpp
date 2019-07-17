#pragma once

//TODO: .inl stuff

namespace clv::evt{//Maybe move to utils?
	template<typename FunctionPrototype>
	class Delegate{
		//VARIABLES
	private:
		FunctionPrototype* function;

		//FUNCTIONS
	public:
		//TODO: Constructors and stuff

		void bind(FunctionPrototype* function){
			this->function = function;
		}

		void broadcast(){
			function();
		}
	};
}

#include "Delegate.inl"