#pragma once

namespace clv::plt{
	struct WindowProps{
		std::string title;
		uint32 width;
		uint32 height;

		WindowProps(const std::string& inTitle = "Clove Engine", uint32 inWidth = 1280u, uint32 inHeight = 720u)
			: title(inTitle)
			, width(inWidth)
			, height(inHeight){
		}
	};
}