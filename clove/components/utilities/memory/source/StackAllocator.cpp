#include "Clove/Memory/StackAllocator.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <cstdlib>

namespace garlic::clove {
    StackAllocator::StackAllocator(size_t sizeBytes)
        : stackSize(sizeBytes)
        , freeMemory(true) {
#if CLV_ENABLE_MEMORY_DEBUGGING
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Constructing new StackAllocator. Size {0}. ", stackSize);
#endif
        stack = reinterpret_cast<std::byte *>(malloc(stackSize));
        top   = stack;
    }

    StackAllocator::StackAllocator(std::byte *start, size_t sizeBytes)
        : stackSize(sizeBytes)
        , freeMemory(false) {
        stack = start;
        top   = stack;
    }

    StackAllocator::StackAllocator(StackAllocator &&other) noexcept = default;

    StackAllocator &StackAllocator::operator=(StackAllocator &&other) noexcept = default;

    StackAllocator::~StackAllocator() {
        if(freeMemory) {
#if GARLIC_DEBUG
            if(top > stack) {
                GARLIC_LOG(garlicLogContext, LogLevel::Warning, "Stack Allocator destructed with active memory. Block will be freed but destructors will not be called on occupying elements");
            }
#endif
            ::free(stack);
        }
    }

    StackAllocator::Marker StackAllocator::markPosition() {
        return top - stack;
    }

    void *StackAllocator::alloc(size_t bytes) {
        if((top - stack) + bytes > stackSize) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "{0}: Not enough space left to allocate {1} bytes.", GARLIC_FUNCTION_NAME_PRETTY, bytes);
            return nullptr;
        }

        void *element = top;
        top += bytes;
        return element;
    }

    void StackAllocator::free() {
        top = stack;
    }

    void StackAllocator::free(Marker marker) {
        top = stack + marker;
    }
}