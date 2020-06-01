#include "Clove/Memory/StackAllocator.hpp"

namespace clv::mem {
	StackAllocator::StackAllocator(size_t sizeBytes)
		: stackSize(sizeBytes)
		, freeMemory(true) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		CLV_LOG_TRACE("Constructing new StackAllocator. Size {0}. ", stackSize);
#endif
		stack = reinterpret_cast<char*>(malloc(stackSize));
		top = stack;
	}

	StackAllocator::StackAllocator(char* start, size_t sizeBytes)
		: stackSize(sizeBytes)
		, freeMemory(false) {
		stack = start;
		top = stack;
	}

	StackAllocator::StackAllocator(StackAllocator&& other) noexcept = default;

	StackAllocator& StackAllocator::operator=(StackAllocator&& other) noexcept = default;

	StackAllocator::~StackAllocator() {
		if(freeMemory) {
			::free(stack);
		}
	}

	StackAllocator::Marker StackAllocator::markPosition() {
		return top - stack;
	}

	void* StackAllocator::alloc(size_t bytes) {
		if((top - stack) + bytes > stackSize) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Not enough space left to allocate {1} bytes.", CLV_FUNCTION_NAME_PRETTY, bytes);
			return nullptr;
		}

		void* element = top;
		top += bytes;
		return element;
	}

	void StackAllocator::free() {
		top = stack;
	}

	void StackAllocator::free(Marker marker) {
		top = stack + marker;
	}
}