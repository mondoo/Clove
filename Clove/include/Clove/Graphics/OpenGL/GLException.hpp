#pragma once

#include <Root/Definitions.hpp>

namespace clv::gfx::ogl{
	void GARLIC_APIENTRY errorCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* msg, const void* data);
}
