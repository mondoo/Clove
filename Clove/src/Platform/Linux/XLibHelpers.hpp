#pragma once

namespace clv::gfx{
	enum class API;
}

namespace clv::XLibHelpers{
	XVisualInfo* getVisualInfo(Display* display, gfx::API api);
}