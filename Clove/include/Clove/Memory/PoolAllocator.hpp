#pragma once

namespace clv::mem {
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
		char* pool;
		size_t numElements;
		PoolItem* nextFree = nullptr;

		bool freeMemory = true;

		//FUNCTIONS
	public:
		PoolAllocator() = delete;
		PoolAllocator(size_t numElements);
		PoolAllocator(char* start, size_t numElements);

		PoolAllocator(const PoolAllocator& other) = delete;
		PoolAllocator(PoolAllocator&& other) noexcept;

		PoolAllocator& operator=(const PoolAllocator& other) = delete;
		PoolAllocator& operator=(PoolAllocator&& other) noexcept;

		~PoolAllocator();

		template<typename... Args>
		ItemType* alloc(Args&&... args);

		void free(ItemType* item);
	};
}

#include "PoolAllocator.inl"