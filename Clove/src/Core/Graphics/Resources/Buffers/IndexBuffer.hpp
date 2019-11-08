#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx{
	struct IndexBufferDescriptor{
		uint32 elementsize = 0;
		uint32 bufferSize = 0;
		uint32 indexCount = 0;
	};
}

namespace clv::gfx{
	class IndexBuffer : public Buffer{
		//FUNCTIONS
	public:
		virtual uint32 getIndexCount() const = 0;
	};
}