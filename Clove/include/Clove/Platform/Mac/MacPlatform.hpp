#pragma once

#include "Clove/Platform/Core/Platform.hpp"

namespace clv::plt{
	class MacPlatform : public Platform{
		//VARIABLES
	private:
		std::unique_ptr<gfx::GraphicsFactory> graphicsFactory;

		//FUNCTIONS
	public:
		MacPlatform() = delete;
		MacPlatform(gfx::API api);

		MacPlatform(const MacPlatform& other) = delete;
		MacPlatform(MacPlatform&& other) noexcept;

		MacPlatform& operator=(const MacPlatform& other) = delete;
		MacPlatform& operator=(MacPlatform&& other) noexcept;

		virtual ~MacPlatform();

		virtual gfx::GraphicsFactory& getGraphicsFactory() override;

		virtual std::shared_ptr<Window> createWindow(const WindowDescriptor& props) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) override;
	};
}
