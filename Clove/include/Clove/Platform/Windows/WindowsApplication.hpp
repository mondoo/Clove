#pragma once

#include "Clove/Core/Platform/Application.hpp"

namespace clv::plt{
	class WindowsApplication : public Application{
		//FUNCTIONS
	public:
		WindowsApplication(gfx::API api);

	private:
		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}