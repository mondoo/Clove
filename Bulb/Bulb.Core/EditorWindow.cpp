#include "EditorWindow.hpp"

#include <Clove/Core/Graphics/RenderCommand.hpp>
#include <Clove/Core/Graphics/Surface.hpp>
#include <Clove/Platform/Windows/CloveWindows.hpp>

namespace clv::plt::blb{
	EditorWindow::EditorWindow(System::IntPtr hWnd, int width, int height){
		handle = reinterpret_cast<HWND>(hWnd.ToPointer());

		data = { handle, static_cast<clv::uint32>(width), static_cast<clv::uint32>(height) };

		surface = clv::gfx::RenderCommand::createSurface(&data);
		clv::gfx::RenderCommand::makeSurfaceCurrent(surface);
	}

	void* EditorWindow::getNativeWindow() const{
		return handle;
	}
}