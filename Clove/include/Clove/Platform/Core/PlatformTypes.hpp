#pragma once

namespace clv::plt{
	struct WindowProps{
		std::string title;
		int32_t width;
		int32_t height;

		WindowProps(const std::string& inTitle = "Clove Engine", int32_t inWidth = 1280, int32_t inHeight = 720)
			: title(inTitle)
			, width(inWidth)
			, height(inHeight){
		}
	};
}