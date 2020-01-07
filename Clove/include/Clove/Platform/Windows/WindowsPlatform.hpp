#pragma once

#include "Clove/Platform/Core/Platform.hpp"

namespace clv::plt{
	class WindowsPlatform : public Platform{
		//FUNCTIONS
	public:
		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}