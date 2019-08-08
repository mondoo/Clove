#pragma once

namespace clv::gfx{
	void APIENTRY errorCallback(uint32 source, uint32 type, uint32 id,
					   uint32 severity, int32 length,
					   const char* msg, const void* data);
}
