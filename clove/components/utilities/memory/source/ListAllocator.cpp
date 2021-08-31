#include "Clove/Memory/ListAllocator.hpp"

#include <Clove/Log/Log.hpp>
#include <cstdlib>

namespace clove {
    ListAllocator::ListAllocator(size_t sizeBytes)
        : backingMemorySize(sizeBytes) {
        backingMemory = reinterpret_cast<std::byte *>(malloc(backingMemorySize));
        freeList.emplace_back(true, 0, 0, backingMemorySize);
    }

    ListAllocator::ListAllocator(std::byte *start, size_t sizeBytes)
        : backingMemorySize{ sizeBytes }
        , freeMemory{ false } {
        backingMemory = reinterpret_cast<std::byte *>(start);
        freeList.emplace_back(true, 0, 0, backingMemorySize);
    }

    ListAllocator::ListAllocator(ListAllocator &&other) noexcept
        : backingMemory{ other.backingMemory }
        , backingMemorySize{ other.backingMemorySize }
        , freeList{ std::move(other.freeList) }
        , freeMemory{ other.freeMemory } {
        other.freeMemory = false;
    }

    ListAllocator &ListAllocator::operator=(ListAllocator &&other) noexcept {
        backingMemory     = other.backingMemory;
        backingMemorySize = other.backingMemorySize;
        freeList          = std::move(freeList);
        freeMemory        = other.freeMemory;

        other.freeMemory = false;

        return *this;
    }

    ListAllocator::~ListAllocator() {
        if(freeMemory) {
            ::free(backingMemory);
        }
    }

    void *ListAllocator::alloc(size_t size, size_t alignment) {
        size_t const totalAllocationSize{ size + alignment };

        for(auto block{ freeList.begin() }; block != freeList.end(); ++block) {
            if(block->free && block->size >= totalAllocationSize) {
                block->free = false;

                if(size_t const remainingSpace{ block->size - totalAllocationSize }; remainingSpace > 0) {
                    block->size = totalAllocationSize;

                    freeList.insert(std::next(block), Block{
                                                          .offset = block->offset + block->size,
                                                          .size   = remainingSpace,
                                                      });
                }

                size_t const remainingAlignment{ alignment != 0 ? block->offset % alignment : 0 };
                size_t const alignmentOffset{ remainingAlignment != 0 ? alignment - remainingAlignment : 0 };

                block->alignedOffset = block->offset + alignmentOffset;

                return backingMemory + block->alignedOffset;
            }
        }

        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Not enough space left to allocate {1} bytes.", CLOVE_FUNCTION_NAME_PRETTY, size);
        return nullptr;
    }

    void ListAllocator::free(void *ptr) {
        if(ptr == nullptr) {
            return;
        }

        for(auto block{ freeList.begin() }; block != freeList.end(); ++block) {
            if(backingMemory + block->alignedOffset == ptr) {
                block->free = true;

                bool removeIndex{ false };
                bool removeRight{ false };

                if(auto rightBlock{ std::next(block) }; rightBlock != freeList.end() && rightBlock->free) {
                    block->size += rightBlock->size;
                    removeRight = true;
                }
                if(block != freeList.begin()) {
                    if(auto leftBlock{ std::prev(block) }; leftBlock->free) {
                        leftBlock->size += block->size;
                        removeIndex = true;
                    }
                }

                if(removeRight) {
                    freeList.erase(std::next(block));
                }
                if(removeIndex) {
                    freeList.erase(block);
                }

                break;
            }
        }
    }
}
