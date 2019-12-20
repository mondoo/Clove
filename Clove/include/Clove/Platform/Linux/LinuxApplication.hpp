#pragma once

#include "Clove/Core/Platform/Application.hpp"

namespace clv::plt{
	class LinuxApplication : public Application{
		//FUNCTIONS
	private:
		virtual gfx::API getPlatformPreferedAPI() override;
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps()) override;
	};
}
