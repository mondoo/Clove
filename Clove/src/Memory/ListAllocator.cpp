#include "Clove/Memory/ListAllocator.hpp"

namespace clv::mem {
	ListAllocator::ListAllocator(size_t sizeBytes)
		: listSize(sizeBytes)
		, freeMemory(true) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		CLV_LOG_TRACE("Constructing new ListAllocator. Size {0}. ", listSize);
#endif
		rawList = reinterpret_cast<char*>(malloc(listSize));
		head = rawList;
	}

	ListAllocator::ListAllocator(char* start, size_t sizeBytes)
		: listSize(sizeBytes)
		, freeMemory(false) {
		rawList = reinterpret_cast<char*>(start);
		head = rawList;
	}

	ListAllocator::ListAllocator(ListAllocator&& other) noexcept {
		rawList = other.rawList;
		listSize = other.listSize;
		head = other.head;

		list = std::move(list);

		freeMemory = other.freeMemory;
		other.freeMemory = false;
	}

	ListAllocator& ListAllocator::operator=(ListAllocator&& other) noexcept {
		rawList = other.rawList;
		listSize = other.listSize;
		head = other.head;

		list = std::move(list);

		freeMemory = other.freeMemory;
		other.freeMemory = false;

		return *this;
	}

	ListAllocator::~ListAllocator() {
		if(freeMemory) {
			::free(rawList);
		}
	}

	void* ListAllocator::alloc(size_t bytes) {
		Header* header = nullptr;
		for(auto* freeHeader : list) { //Finding first available
			if(bytes <= freeHeader->blockSize) {
				header = freeHeader;
				list.remove(header);
				break;
				//TODO: Can reduce fragmentation by putting remaining bytes back in the list
			}
		}

		if(header == nullptr) {
			if((head - rawList) + bytes > listSize) {
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Not enough space left to allocate {1} bytes.", CLV_FUNCTION_NAME_PRETTY, bytes);
				return nullptr;
			}

			header = reinterpret_cast<Header*>(head);
			header->blockSize = bytes;
			head += sizeof(Header) + bytes;
		}

		return reinterpret_cast<char*>(header) + sizeof(Header);
	}

	void ListAllocator::free(void* ptr) {
		char* data		= reinterpret_cast<char*>(ptr);
		Header* header	= reinterpret_cast<Header*>(data - sizeof(Header));

		list.push_back(header);
	}
}
