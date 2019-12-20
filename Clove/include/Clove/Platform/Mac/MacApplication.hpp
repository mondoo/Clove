#pragma once

#include "Clove/Core/Platform/Application.hpp"

namespace clv::plt{
	class MacApplication : public Application{
		//FUNCTIONS
	public:
		MacApplication();
		
	private:
		virtual gfx::API getPlatformPreferedAPI() override;
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps()) override;
	};
}
