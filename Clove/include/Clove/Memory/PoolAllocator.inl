namespace clv::mem {
	template<typename ItemType>
	PoolAllocator<ItemType>::PoolArena::PoolArena(size_t size) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		this->size = size;
		CLV_LOG_TRACE("New PoolAllocator arena constructed. can contain {0} item(s), a total of {1} bytes", size, sizeof(ItemType) * size);
#endif
		storage = std::unique_ptr<PoolItem[]>{ new PoolItem[size] };
		for(size_t i = 1; i < size; ++i) {
			storage[i - 1].next = &storage[i];
		}
		storage[size - 1].next = nullptr;
	}

#if CLV_ENABLE_MEMORY_DEBUGGING
	template<typename ItemType>
	PoolAllocator<ItemType>::PoolArena::~PoolArena() {
		CLV_LOG_TRACE("PollAllocator arena destructed. {0} bytes were freed", sizeof(ItemType) * size);
	}
#else
	template<typename ItemType>
	PoolAllocator<ItemType>::PoolArena::~PoolArena() = default;
#endif

	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(size_t elementsPerArena)
		: arenaSize(elementsPerArena) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		CLV_LOG_TRACE("Constructing new PoolAllocator. Arena size {0}. ", arenaSize);
#endif
		arena = std::make_unique<PoolArena>(arenaSize);
		nextFree = &arena->storage[0];
	}

	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(PoolAllocator&& other) noexcept = default;

	template<typename ItemType>
	PoolAllocator<ItemType>& PoolAllocator<ItemType>::operator=(PoolAllocator&& other) noexcept = default;

	template<typename ItemType>
	template<typename... Args>
	ItemType* PoolAllocator<ItemType>::alloc(Args&&... args) {
		if(!nextFree) {
	#if CLV_ENABLE_MEMORY_DEBUGGING
			CLV_LOG_TRACE("PoolAllocator has run out of free items. Constructing a new arena...");
	#endif
			std::unique_ptr<PoolArena> newArena = std::make_unique<PoolArena>(arenaSize);
			newArena->next = std::move(arena);
			arena = std::move(newArena);
			nextFree = &arena->storage[0];
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