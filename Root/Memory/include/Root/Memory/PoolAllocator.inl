#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace garlic::inline root {
    template<typename ItemType>
    PoolAllocator<ItemType>::PoolAllocator(size_t numElements)
        : numElements(numElements) {
#if CLV_ENABLE_MEMORY_DEBUGGING
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Constructing new PoolAllocator. Arena size {0}. ", arenaSize);
#endif
        pool     = reinterpret_cast<std::byte*>(malloc(numElements * sizeof(PoolItem)));
        nextFree = reinterpret_cast<PoolItem*>(pool);

        PoolItem* iter = reinterpret_cast<PoolItem*>(pool);
        for(size_t i = 1; i < numElements; ++i) {
            iter[i - 1].next = &iter[i];
        }
        iter[numElements - 1].next = nullptr;
    }

    template<typename ItemType>
    PoolAllocator<ItemType>::PoolAllocator(PoolAllocator&& other) noexcept = default;

    template<typename ItemType>
    PoolAllocator<ItemType>& PoolAllocator<ItemType>::operator=(PoolAllocator&& other) noexcept = default;

    template<typename ItemType>
    PoolAllocator<ItemType>::~PoolAllocator() {
#if GARLIC_DEBUG
        size_t availableElements = 0;
        PoolItem* item           = nextFree;

        while(item != nullptr) {
            ++availableElements;
            item = item->next;
        }

        if(availableElements < numElements) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "Pool Allocator destructed with active memory. Block will be freed but destructors will not be called on occupying elements");
        }
#endif
        ::free(pool);
    }

    template<typename ItemType>
    template<typename... Args>
    ItemType* PoolAllocator<ItemType>::alloc(Args&&... args) {
        if(nextFree == nullptr) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "{0}: At the end of the free list. Cannot allocate new elements", GARLIC_FUNCTION_NAME_PRETTY);
            return nullptr;
        }

        PoolItem* poolItem = nextFree;
        nextFree           = poolItem->next;

        ItemType* item = reinterpret_cast<ItemType*>(poolItem->item);
        new(item) ItemType(std::forward<Args>(args)...);

        return item;
    }

    template<typename ItemType>
    void PoolAllocator<ItemType>::free(ItemType* item) {
        item->~ItemType();

        PoolItem* poolItem = reinterpret_cast<PoolItem*>(item);
        poolItem->next     = nextFree;
        nextFree           = poolItem;
    }
}