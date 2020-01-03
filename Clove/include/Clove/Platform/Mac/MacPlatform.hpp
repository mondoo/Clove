#pragma once

#include "Clove/Core/Platform/Platform.hpp"

namespace clv::plt{
	class MacPlatform : public Platform{
		//FUNCTIONS
	public:
		MacPlatform();

		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}
