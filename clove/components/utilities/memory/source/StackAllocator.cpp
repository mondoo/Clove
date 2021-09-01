#include "Clove/Memory/StackAllocator.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <cstdlib>

namespace clove {
    StackAllocator::StackAllocator(size_t sizeBytes)
        : stackSize(sizeBytes) {
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
            ::free(stack);
        }
    }

    StackAllocator::Marker StackAllocator::markPosition() {
        return top - stack;
    }

    void *StackAllocator::alloc(size_t size, size_t alignment) {
        size_t const totalAllocationSize{ size + alignment };

        if((top - stack) + totalAllocationSize > stackSize) {
            CLOVE_LOG(Clove, LogLevel::Error, "{0}: Not enough space left to allocate {1} bytes.", CLOVE_FUNCTION_NAME_PRETTY, totalAllocationSize);
            return nullptr;
        }

        size_t const remainingAlignment{ alignment != 0 ? reinterpret_cast<uintptr_t>(top) % alignment : 0 };
        size_t const alignmentOffset{ remainingAlignment != 0 ? alignment - remainingAlignment : 0 };

        void *element{ top + alignmentOffset };
        top += totalAllocationSize;

        return element;
    }

    void StackAllocator::free() {
        top = stack;
    }

    void StackAllocator::free(Marker marker) {
        top = stack + marker;
    }
}