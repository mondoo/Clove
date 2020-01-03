#pragma once

namespace clv::plt{
	struct WindowProps{
		std::string title;
		int32 width;
		int32 height;

		WindowProps(const std::string& inTitle = "Clove Engine", int32 inWidth = 1280u, int32 inHeight = 720u)
			: title(inTitle)
			, width(inWidth)
			, height(inHeight){
		}
	};

	enum class WindowType{
		MainWindow,
		SubWindow
	};
}