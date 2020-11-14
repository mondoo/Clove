#pragma once

#include "Clove/Platform/Platform.hpp"

namespace clv::plt {
    class WindowsPlatform : public Platform {
        //FUNCTIONS
    public:
        WindowsPlatform();

        WindowsPlatform(WindowsPlatform const &other) = delete;
        WindowsPlatform(WindowsPlatform &&other) noexcept;

        WindowsPlatform &operator=(WindowsPlatform const &other) = delete;
        WindowsPlatform &operator=(WindowsPlatform &&other) noexcept;

        ~WindowsPlatform();

        std::shared_ptr<Window> createWindow(WindowDescriptor const &descriptor) override;
        std::shared_ptr<Window> createChildWindow(Window const &parentWindow, mth::vec2i const &position, mth::vec2i const &size) override;
    };
}