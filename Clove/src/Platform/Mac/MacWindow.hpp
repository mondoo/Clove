#pragma once

#include "Clove/Platform/Window.hpp"

namespace clv{
    class MacWindow : public Window{
        //VARIABLES
    private:
		NSWindow* window;
		NSWindowController* controller;
			
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
		
		virtual void* getNativeWindow() const override;
		
	protected:
		virtual void processInput() override;
		
	private:
		void initialiseWindow(const WindowProps& props, gfx::API api);
    };
}
