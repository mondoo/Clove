#include "Clove/Memory/StackAllocator.hpp"

namespace clv::mem {
	StackAllocator::StackAllocator(size_t sizeBytes) {
		stack = std::unique_ptr<char[]>{ new char[sizeBytes] };
		top = &stack[0];
	}

	StackAllocator::StackAllocator(StackAllocator&& other) noexcept = default;

	StackAllocator& StackAllocator::operator=(StackAllocator&& other) noexcept = default;

	StackAllocator::~StackAllocator() = default;

	StackAllocator::Marker StackAllocator::markPosition() {
		return top - &stack[0];
	}

	void* StackAllocator::alloc(size_t bytes) {
		void* element = top;
		top += bytes;
		return element;
	}

	void StackAllocator::free() {
		top = &stack[0];
	}

	void StackAllocator::free(Marker marker) {
		top = &stack[0] + marker;
	}
}