#pragma once

#include "Clove/Platform/Window"

namespace clv{
    class MacWindow : public Window{
        //VARIABLES
    private:
        
        //FUNCTIONS
    public:
        MacWindow() = delete;
        MacWindow(const MacWindow& other) = delete;
        MacWindow(MacWindow&& other) noexcept = delete;
        MacWindow& operator=(const MacWindow& other) = delete;
        MacWindow& operator=(MacWindow&& other) noexcept = delete;
        virtual ~MacWindow();
        
        MacWindow(const WindowProps& props);
        MacWindow(const WindowProps& props, gfx::API api);
    };
}
