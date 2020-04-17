#include "EditorWindowProxy.hpp"

namespace blb::plt{
	EditorWindowProxy::EditorWindowProxy(System::IntPtr hWnd){
		handle = reinterpret_cast<HWND>(hWnd.ToPointer());
	}

	void* EditorWindowProxy::getNativeWindow() const{
		return handle;
	}
}