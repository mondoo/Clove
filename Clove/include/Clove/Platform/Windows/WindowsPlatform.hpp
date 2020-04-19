#pragma once

#include "Clove/Platform/Platform.hpp"

namespace clv::plt{
	class WindowsPlatform : public Platform{
		//FUNCTIONS
	public:
		WindowsPlatform();

		WindowsPlatform(const WindowsPlatform& other) = delete;
		WindowsPlatform(WindowsPlatform&& other) noexcept;

		WindowsPlatform& operator=(const WindowsPlatform& other) = delete;
		WindowsPlatform& operator=(WindowsPlatform&& other) noexcept;

		virtual ~WindowsPlatform();

		virtual std::shared_ptr<Window> createWindow(const WindowDescriptor& descriptor) override;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api) override;
	};
}