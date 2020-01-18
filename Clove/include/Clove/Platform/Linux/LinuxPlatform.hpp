#pragma once

#include "Clove/Platform/Core/Platform.hpp"

namespace clv::plt{
	class LinuxPlatform : public Platform{
		//VARIABLES
	private:
		std::unique_ptr<gfx::GraphicsFactory> graphicsFactory;

		//FUNCTIONS
	public:
		LinuxPlatform() = delete;
		LinuxPlatform(gfx::API api);

		LinuxPlatform(const LinuxPlatform& other) = delete;
		LinuxPlatform(LinuxPlatform&& other) noexcept;

		LinuxPlatform& operator=(const LinuxPlatform& other) = delete;
		LinuxPlatform& operator=(LinuxPlatform&& other) noexcept;

		virtual ~LinuxPlatform();

		virtual gfx::GraphicsFactory& getGraphicsFactory() override;

		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
        virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}
