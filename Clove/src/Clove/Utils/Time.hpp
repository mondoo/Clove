#pragma once

namespace clv{
	class Application;
}

namespace clv::utl{
	class Time{
		friend class clv::Application;

		//VARIABLES
	private:
		static float deltaTime;

		//FUNCTIONS
	public:
		static float getDeltaTime();
	};
}