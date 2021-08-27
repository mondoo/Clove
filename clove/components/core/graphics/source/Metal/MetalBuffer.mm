#include "Clove/Graphics/Metal/MetalBuffer.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
	MetalBuffer::MetalBuffer(id<MTLBuffer> buffer, Descriptor descriptor)
		: buffer{ buffer }
		, descriptor{ descriptor }{
	}

	MetalBuffer::MetalBuffer(MetalBuffer &&other) noexcept = default;

	MetalBuffer &MetalBuffer::operator=(MetalBuffer &&other) noexcept = default;

	MetalBuffer::~MetalBuffer() = default;

	GhaBuffer::Descriptor const &MetalBuffer::getDescriptor() const {
        return descriptor;
    }

	void MetalBuffer::write(void const *data, size_t const offset, size_t const size) {
		uint8_t *cpuAccessibleMemory{ reinterpret_cast<uint8_t *>([buffer contents]) };
		memcpy((cpuAccessibleMemory + offset), data, size);
	}

	void MetalBuffer::read(void *data, size_t const offset, size_t const size) {
		uint8_t *cpuAccessibleMemory{ reinterpret_cast<uint8_t *>([buffer contents]) };
		memcpy(data, (cpuAccessibleMemory + offset), size);
	}

	id<MTLBuffer> MetalBuffer::getBuffer() const {
		return buffer;
	}
}
