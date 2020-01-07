namespace clv::mem{
	template<typename ItemType>
	PoolAllocator<ItemType>::PoolArena::PoolArena(std::size_t size){
		storage = std::unique_ptr<PoolItem[]>{ new PoolItem[size] };
		for(std::size_t i = 1; i < size; ++i){
			storage[i - 1].next = &storage[i];
		}
		storage[size - 1].next = nullptr;
	}

	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(std::size_t arenaSize)
		: arenaSize(arenaSize){
		arena = std::make_unique<PoolArena>(arenaSize);
		nextFree = &arena->storage[0];
	}

	template<typename ItemType>
	PoolAllocator<ItemType>::PoolAllocator(PoolAllocator&& other) noexcept = default;

	template<typename ItemType>
	PoolAllocator<ItemType>& PoolAllocator<ItemType>::operator=(PoolAllocator&& other) noexcept = default;

	template<typename ItemType>
	template<typename ...Args>
	ItemType* PoolAllocator<ItemType>::alloc(Args&& ...args){
		if(!nextFree){
			std::unique_ptr<PoolArena> newArena = std::make_unique<PoolArena>(arenaSize);
			newArena->next = std::move(arena);
			arena = std::move(newArena);
			nextFree = &arena->storage[0];
		}

		PoolItem* poolItem = nextFree;
		nextFree = poolItem->next;

		ItemType* item = reinterpret_cast<ItemType*>(poolItem->item);
		new (item) ItemType(std::forward<Args>(args)...);

		return item;
	}

	template<typename ItemType>
	void PoolAllocator<ItemType>::free(ItemType* item){
		item->~ItemType();

		PoolItem* poolItem = reinterpret_cast<PoolItem*>(item);
		poolItem->next = nextFree;
		nextFree = poolItem;
	}
}