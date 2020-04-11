#pragma once

namespace clv::mem{
	template<typename ItemType>
	class PoolAllocator{
		//TYPES
	private:
		union PoolItem{
			PoolItem* next;
			alignas(alignof(ItemType)) char item[sizeof(ItemType)];
		};

		struct PoolArena{
			//VARIABLES
		public:
			std::unique_ptr<PoolItem[]> storage;
			std::unique_ptr<PoolArena> next;

		private:
		#if CLV_ENABLE_MEMORY_DEBUGGING
			std::size_t size;
		#endif

			//FUNCTIONS
		public:
			PoolArena(std::size_t size);
			~PoolArena();
		};

		//VARIABLES
	private:
		std::size_t arenaSize;
		std::unique_ptr<PoolArena> arena;
		PoolItem* nextFree = nullptr;

		//FUNCTIONS
	public:
		PoolAllocator() = delete;
		PoolAllocator(std::size_t arenaSize);

		PoolAllocator(const PoolAllocator& other) = delete;
		PoolAllocator(PoolAllocator&& other) noexcept;

		PoolAllocator& operator=(const PoolAllocator& other) = delete;
		PoolAllocator& operator=(PoolAllocator&& other) noexcept;

		~PoolAllocator() = default;

		template<typename ...Args>
		ItemType* alloc(Args&& ...args);
		void free(ItemType* item);
	};
}

#include "PoolAllocator.inl"