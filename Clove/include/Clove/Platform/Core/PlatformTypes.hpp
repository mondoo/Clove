#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::plt{
	struct WindowDescriptor{
		std::string title;
		int32 width;
		int32 height;
		gfx::API api;
	};
}