#include "PoolAllocator.hpp"
namespace clv::mem {
	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(size_t numElements)
		: numElements(numElements)
		, freeMemory(true) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		CLV_LOG_TRACE("Constructing new PoolAllocator. Arena size {0}. ", arenaSize);
#endif
		pool = reinterpret_cast<char*>(malloc(numElements * sizeof(PoolItem)));
		nextFree = reinterpret_cast<PoolItem*>(pool);

		PoolItem* iter = reinterpret_cast<PoolItem*>(pool);
		for(size_t i = 1; i < numElements; ++i) {
			iter[i - 1].next = &iter[i];
		}
		iter[numElements - 1].next = nullptr;
	}

	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(char* start, size_t numElements)
		: numElements(numElements)
		, freeMemory(false) {

		pool = start;
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
		if(freeMemory) {
			::free(pool);
		}
	}

	template<typename ItemType>
	template<typename... Args>
	ItemType* PoolAllocator<ItemType>::alloc(Args&&... args) {
		if(nextFree == nullptr) {
			CLV_LOG_ERROR("{0}: At the end of the free list. Cannot allocate new elements", CLV_FUNCTION_NAME_PRETTY);
			return nullptr;
		}

		PoolItem* poolItem = nextFree;
		nextFree = poolItem->next;

		ItemType* item = reinterpret_cast<ItemType*>(poolItem->item);
		new(item) ItemType(std::forward<Args>(args)...);

		return item;
	}

	template<typename ItemType>
	void PoolAllocator<ItemType>::free(ItemType* item) {
		item->~ItemType();

		PoolItem* poolItem = reinterpret_cast<PoolItem*>(item);
		poolItem->next = nextFree;
		nextFree = poolItem;
	}
}