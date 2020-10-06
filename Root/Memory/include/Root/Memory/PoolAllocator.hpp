#pragma once

#include <cstddef>

namespace garlic::inline root {
    /**
     * @brief An allocator that prevents memory being fragmented but can only allocate a single type of item.
     * @details A PoolAllocator can only work on a single type of item because internally it works like 
     * a linked list, inserting new items into free spaces. A PoolAllocator is less flexible than a
     * ListAllocator because it can only allocate a single item but can very easily avoid memory fragmentation.
     */
    template<typename ItemType>
    class PoolAllocator {
        //TYPES
    private:
        union PoolItem {
            PoolItem* next;
            alignas(alignof(ItemType)) char item[sizeof(ItemType)];
        };

        //VARIABLES
    private:
        std::byte* pool;
        size_t numElements;
        PoolItem* nextFree = nullptr;

        bool freeMemory = true;

        //FUNCTIONS
    public:
        PoolAllocator() = delete;
        PoolAllocator(size_t numElements);
        PoolAllocator(std::byte* start, size_t numElements);

        PoolAllocator(const PoolAllocator& other) = delete;
        PoolAllocator(PoolAllocator&& other) noexcept;

        PoolAllocator& operator=(const PoolAllocator& other) = delete;
        PoolAllocator& operator=(PoolAllocator&& other) noexcept;

        ~PoolAllocator();

        /**
         * @brief Allocates a new item of ItemType. Calling it's constructor.
         */
        template<typename... Args>
        ItemType* alloc(Args&&... args);

        /**
         * @brief Frees an item, calling it's destructor.
         */
        void free(ItemType* item);
    };
}

#include "PoolAllocator.inl"