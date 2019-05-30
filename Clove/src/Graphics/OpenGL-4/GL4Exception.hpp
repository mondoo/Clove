#pragma once

namespace clv::gfx{
	void APIENTRY errorCallback(unsigned int source, unsigned int type, unsigned int id,
								unsigned int severity, int length,
								const char *msg, const void *data);
}
