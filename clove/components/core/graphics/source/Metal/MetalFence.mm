#include "Clove/Graphics/Metal/MetalFence.hpp"

namespace garlic::clove {
	MetalFence::MetalFence(dispatch_semaphore_t semaphore)
		: semaphore{ [semaphore retain] } {
	}
	
	MetalFence::MetalFence(MetalFence &&other) noexcept = default;
	
	MetalFence &MetalFence::operator=(MetalFence &&other) noexcept = default;
	
	MetalFence::~MetalFence() {
		[semaphore release];
	}
	
	void MetalFence::wait() {
		dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
	}
	
	void MetalFence::reset() {
		//No op
		/*
		 These semaphores work by decrementing a count and if it's below 0 it waits.
		 Once a command buffer signals the semaphore it increase the count by 1. Therefore
		 the semaphore automatically resets itself in a way
		 */
	}
}
