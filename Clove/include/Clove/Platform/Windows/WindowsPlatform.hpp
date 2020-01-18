#pragma once

#include "Clove/Platform/Core/Platform.hpp"

namespace clv::plt{
	class WindowsPlatform : public Platform{
		//VARIABLES
	private:
		std::unique_ptr<gfx::GraphicsFactory> graphicsFactory;

		//FUNCTIONS
	public:
		WindowsPlatform() = delete;
		WindowsPlatform(gfx::API api);

		WindowsPlatform(const WindowsPlatform& other) = delete;
		WindowsPlatform(WindowsPlatform&& other) noexcept;

		WindowsPlatform& operator=(const WindowsPlatform& other) = delete;
		WindowsPlatform& operator=(WindowsPlatform&& other) noexcept;

		virtual ~WindowsPlatform();

		virtual gfx::GraphicsFactory& getGraphicsFactory() override;

		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}