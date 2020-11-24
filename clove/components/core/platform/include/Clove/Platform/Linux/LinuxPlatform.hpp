#pragma once

#include "Clove/Platform/Platform.hpp"

namespace garlic::clove {
    class LinuxPlatform : public Platform {
        //FUNCTIONS
    public:
        LinuxPlatform();

        LinuxPlatform(LinuxPlatform const &other) = delete;
        LinuxPlatform(LinuxPlatform &&other) noexcept;

        LinuxPlatform &operator=(LinuxPlatform const &other) = delete;
        LinuxPlatform &operator=(LinuxPlatform &&other) noexcept;

        ~LinuxPlatform();

        std::shared_ptr<Window> createWindow(WindowDescriptor const &props) override;
        std::shared_ptr<Window> createChildWindow(Window const &parentWindow, vec2i const &position, vec2i const &size) override;
    };
}
