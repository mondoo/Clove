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

	protected:
		virtual void processInput() override{}
	};
}