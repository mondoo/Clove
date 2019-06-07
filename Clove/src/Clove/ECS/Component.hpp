#pragma once

//components hold data

namespace clv::ecs{
	class Component{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		virtual void init() = 0; //init on construct?

	};
}