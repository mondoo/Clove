#pragma once

#include "Core/Platform/Application.hpp"

namespace clv::plt{
	class MacApplication : public Application{
		//FUNCTIONS
	public:
		MacApplication();
		
	private:
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps()) override;
		virtual gfx::API getPlatformPreferedAPI() override;
	};
}
