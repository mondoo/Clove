#pragma once

#include "Clove/Graphics/FactoryChild.hpp"

namespace clv::gfx {
	enum class BufferType {
		IndexBuffer,
		VertexBuffer,
		ShaderResourceBuffer
	};

	enum class BufferUsage {
		Default,
		Dynamic
	};

	struct BufferDescriptor {
		std::size_t elementSize = 0;
		std::size_t bufferSize = 0;
		BufferType bufferType;
		BufferUsage bufferUsage;
	};
}

namespace clv::gfx {
	class Buffer : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~Buffer() = default;

		virtual const BufferDescriptor& getDescriptor() const = 0;

		virtual void updateData(const void* data) = 0;
	};
}