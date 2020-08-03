#pragma once

#include <list>
#include <cstddef>

namespace clv::mem {
    /**
     * @brief An allocator that prevents memory fragmentation that works for any size of allocation.
     * @details A ListAllocator is a bit more complicated than a PoolAllocator because
     * it has to keep track of each block of free memory and how big that block is. It is
     * more difficult to completely avoid fragmentation but is more flexible because any
     * amount of memory can be allocated.
     */
    class ListAllocator {
        //TYPES
    private:
        struct Header {
            size_t blockSize = 0;
        };

        //VARIABLES
    private:
        std::byte* rawList;
        size_t listSize = 0;
        std::byte* head = nullptr;

        std::list<Header*> list;

        bool freeMemory = true;

#if GARLIC_DEBUG
        uint32_t allocations = 0;
        uint32_t frees       = 0;
#endif

        //FUNCTIONS
    public:
        ListAllocator() = delete;
        ListAllocator(size_t sizeBytes);
        ListAllocator(std::byte* start, size_t sizeBytes);

        ListAllocator(const ListAllocator& other) = delete;
        ListAllocator(ListAllocator&& other) noexcept;

        ListAllocator& operator=(const ListAllocator& other) = delete;
        ListAllocator& operator=(ListAllocator&& other) noexcept;

        ~ListAllocator();

        /**
         * @brief Allocates X bytes from the list.
         * @returns A Pointer to the allocated block of memory.
         */
        void* alloc(size_t bytes);

        void free(void* ptr);
    };
}