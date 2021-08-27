#include "Clove/Graphics/Validation/ValidationBuffer.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    template<typename BaseBufferType>
    void ValidationBuffer<BaseBufferType>::write(void const *data, size_t const offset, size_t const size) {
        CLOVE_ASSERT(BaseBufferType::getDescriptor().memoryType == MemoryType::SystemMemory, "{0}: Can only write to SystemMemory buffers", CLOVE_FUNCTION_NAME_PRETTY);

        BaseBufferType::write(data, offset, size);
    }

    template<typename BaseBufferType>
    void ValidationBuffer<BaseBufferType>::read(void *data, size_t const offset, size_t const size) {
        CLOVE_ASSERT(BaseBufferType::getDescriptor().memoryType == MemoryType::SystemMemory, "{0}: Can only read from SystemMemory buffers", CLOVE_FUNCTION_NAME_PRETTY);

        BaseBufferType::read(data, offset, size);
    }
}