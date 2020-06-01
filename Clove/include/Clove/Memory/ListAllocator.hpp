#pragma once

#include <list>

namespace clv::mem {
	class ListAllocator {
		//TYPES
	private:
		struct Header {
			size_t blockSize = 0;
		};

		//VARIABLES
	private:
		char* rawList;
		size_t listSize = 0;
		char* head = nullptr;

		std::list<Header*> list;

		bool freeMemory = true;

		//FUNCTIONS
	public:
		ListAllocator() = delete;
		ListAllocator(size_t sizeBytes);
		ListAllocator(char* start, size_t sizeBytes);

		ListAllocator(const ListAllocator& other) = delete;
		ListAllocator(ListAllocator&& other) noexcept;

		ListAllocator& operator=(const ListAllocator& other) = delete;
		ListAllocator& operator=(ListAllocator&& other) noexcept;

		~ListAllocator();

		void* alloc(size_t bytes);

		void free(void* ptr);
	};
}