#pragma once

namespace clv::plt{
	struct WindowDescriptor{
		std::string title;
		int32 width;
		int32 height;

		WindowDescriptor(const std::string& inTitle = "Clove Engine", int32 inWidth = 1280u, int32 inHeight = 720u)
			: title(inTitle)
			, width(inWidth)
			, height(inHeight){
		}
	};
}