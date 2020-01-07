#pragma once

//TODO: inl

namespace clv::mem{
	template<typename ItemType>
	class PoolAllocator{
	private:
		union PoolItem{
			PoolItem* next;
			alignas(alignof(ItemType)) char item[sizeof(ItemType)];
		};

		class PoolArena{
			//VARIABLES
		public:
			std::unique_ptr<PoolItem[]> storage;
			std::unique_ptr<PoolArena> next;

			//FUNCTIONS
		public:
			PoolArena(std::size_t size){
				storage = std::unique_ptr<PoolItem[]>{ new PoolItem[size] };
				for(std::size_t i = 1; i < size; ++i){
					storage[i - 1].next = &storage[i];
				}
				storage[size - 1].next = nullptr;
			}
		};


		//VARIABLES
	private:
		std::size_t arenaSize;
		std::unique_ptr<PoolArena> arena;
		PoolItem* nextFree = nullptr;

		//FUNCTIONS
	public:
		PoolAllocator() = delete;
		PoolAllocator(std::size_t arenaSize)
			: arenaSize(arenaSize){
			arena = std::make_unique<PoolArena>(arenaSize);
			nextFree = &arena->storage[0];
		}

		//TODO: others

		~PoolAllocator() = default;

		template<typename ...Args>
		ItemType* alloc(Args&& ...args){
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

		void free(ItemType* item){
			item->~ItemType();

			PoolItem* poolItem = reinterpret_cast<PoolItem*>(item);
			poolItem->next = nextFree;
			nextFree = poolItem;
		}
	};
}

#include "PoolAllocator.inl"