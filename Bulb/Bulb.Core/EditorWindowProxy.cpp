#include "EditorWindowProxy.hpp"

namespace clv::plt::blb{
	EditorWindowProxy::EditorWindowProxy(System::IntPtr hWnd){
		handle = reinterpret_cast<HWND>(hWnd.ToPointer());
	}

	void* EditorWindowProxy::getNativeWindow() const{
		return handle;
	}
}