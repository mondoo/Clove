#include "Clove/Memory/StackAllocator.hpp"

namespace clv::mem {
	StackAllocator::StackAllocator(size_t sizeBytes) 
		: stackSize(sizeBytes) {
#if CLV_ENABLE_MEMORY_DEBUGGING
		CLV_LOG_TRACE("Constructing new StackAllocator. Size {0}. ", stackSize);
#endif
		stack = std::unique_ptr<char[]>{ new char[stackSize] };
		top = &stack[0];
	}

	StackAllocator::StackAllocator(StackAllocator&& other) noexcept = default;

	StackAllocator& StackAllocator::operator=(StackAllocator&& other) noexcept = default;

	StackAllocator::~StackAllocator() = default;

	StackAllocator::Marker StackAllocator::markPosition() {
		return top - &stack[0];
	}

	void* StackAllocator::alloc(size_t bytes) {
		if((top - &stack[0]) + bytes <= stackSize) {
			void* element = top;
			top += bytes;
			return element;
		} else {
			CLV_LOG_ERROR("{0}: Not enough space left to allocate {1} bytes.", CLV_FUNCTION_NAME_PRETTY, bytes);
			return nullptr;
		}
	}

	void StackAllocator::free() {
		top = &stack[0];
	}

	void StackAllocator::free(Marker marker) {
		top = &stack[0] + marker;
	}
}