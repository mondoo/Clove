#include "Clove/Graphics/Validation/ValidationBuffer.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    ValidationBuffer::ValidationBuffer(Descriptor descriptor, std::unique_ptr<GhaBuffer> buffer)
        : descriptor{ std::move(descriptor) }
        , internalBuffer{ std::move(buffer) } {
    }

    ValidationBuffer::ValidationBuffer(ValidationBuffer &&other) noexcept = default;

    ValidationBuffer &ValidationBuffer::operator=(ValidationBuffer &&other) noexcept = default;

    ValidationBuffer::~ValidationBuffer() = default;

    void ValidationBuffer::write(void const *data, size_t const offset, size_t const size) {
        CLOVE_ASSERT(descriptor.memoryType == MemoryType::SystemMemory, "{0}: Can only write to SystemMemory buffers", CLOVE_FUNCTION_NAME_PRETTY);

        internalBuffer->write(data, offset, size);
    }

    void ValidationBuffer::read(void *data, size_t const offset, size_t const size) {
        CLOVE_ASSERT(descriptor.memoryType == MemoryType::SystemMemory, "{0}: Can only read from SystemMemory buffers", CLOVE_FUNCTION_NAME_PRETTY);

        internalBuffer->read(data, offset, size);
    }
}