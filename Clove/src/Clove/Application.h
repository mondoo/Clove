#pragma once

#include "Core.h"

namespace clv{
	class CLV_API Application{
	public:
		Application() = default;
		virtual ~Application() = default;

		void run();
	};

	//To be defined in CLIENT
	Application* createApplication();
}

