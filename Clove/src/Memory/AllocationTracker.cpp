#if CLV_TRACK_MEMORY_ALLOCATIONS

#include <iostream>

void* operator new(size_t bytes) {
	return malloc(bytes);
}

void operator delete(void* memory) {
	free(memory);
}

#endif