#pragma once

#include "Clove/Core/Platform/Application.hpp"

namespace clv::plt{
	class WindowsApplication : public Application{
		//FUNCTIONS
	private:
		virtual gfx::API getPlatformPreferedAPI() override;
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = {}) override;
		virtual std::unique_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}