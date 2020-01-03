#pragma once

#include "Clove/Core/Platform/Platform.hpp"

namespace clv::plt{
	class LinuxPlatform : public Platform{
		//FUNCTIONS
	public:
		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
        virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}
