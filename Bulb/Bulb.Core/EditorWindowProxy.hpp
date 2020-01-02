#pragma once

#include <Clove/Core/Platform/Window.hpp>

namespace clv::plt::blb{
	class EditorWindowProxy : public clv::plt::Window{
		//VARIABLES
	private:
		HWND handle;

		//FUNCTIONS
	public:
		//TODO: other Ctors

		EditorWindowProxy(System::IntPtr hWnd);

		virtual void* getNativeWindow() const override;

		virtual mth::vec2i getPosition() const{ return { 0, 0 }; };
		virtual mth::vec2i getSize() const{ return { 0, 0 }; };

		virtual void moveWindow(const mth::vec2i& position){};
		virtual void resizeWindow(const mth::vec2i& size){};

	protected:
		virtual void processInput() override{}
	};
}