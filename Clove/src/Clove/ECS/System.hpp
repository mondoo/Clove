#pragma once

//systems provide functionality

namespace clv::ecs{
	class System{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		virtual void init() = 0; //init construct?
		virtual void update(float deltaTime) = 0;
	};
}
